#include "CmdParameters.h"
#include <cassert>
#include <iostream>
#include <cstring>   // strcmp()
#include "Support/Exception.h"
#include "Support/debug.h"
#include "Types/Types.h"
#include "lib_local.h"

using namespace std;

namespace {

enum {
  PAD_OFFSET = 5
};

using List = TypedParameter::List;

string pad(unsigned width, std::string const &str = "") {
  string out;

  for (auto i = str.length(); i < width; ++ i) {
    out += " ";
  }

  return str + out;
}


bool process_option(List &parameters, char const *curarg) {
  for (auto &item: parameters) {
    TypedParameter &param = *item;
    if (param.parse_param(curarg)) return true;
  }

  return false;
}


string set_indent(int indent, string const &str) {
  string str_indent("\n");
  str_indent += pad(indent);
  return Strings::implode(Strings::explode(str, '\n'), str_indent.c_str());
}

}  // anon namespace


// Internal definition of help switch
DefParameter CmdParameters::help_def(
  "Help switch",
  { "help", "-h" },
  NONE,
  "Show this information. Overrides all other parameters"
);


NoneParameter CmdParameters::help_switch(CmdParameters::help_def);

CmdParameters::CmdParameters(CmdParameters const &rhs) : m_usage(rhs.m_usage) {
  init(&rhs);
}

CmdParameters::CmdParameters(char const *in_usage, DefParameters global_params, CmdParameters *parent) :
  m_usage(in_usage),
  global_parameters(global_params) {
  init(parent);
}


CmdParameters::CmdParameters(char const *in_usage, DefActions in_actions, CmdParameters *parent) :
  m_usage(in_usage),
  actions(in_actions) {
  init(parent);
}


CmdParameters::CmdParameters(
  char const *in_usage,
  DefActions in_actions,
  DefParameters global_params,
  CmdParameters *parent) :
  m_usage(in_usage),
  global_parameters(global_params),
  actions(in_actions) {
  init(parent);
}


/**
 * @brief Create help usage text for all the defined global parameters.
 *
 * Output per option has the form:
 *
 *   switch[=<param_type>] description[<text for default>]
 *
 * Some effort is done to keep the descriptions aligned.
 */
std::string CmdParameters::get_usage() const {
  std::string ret;
  ret += m_usage;
  ret += get_actions();
  ret += get_params(m_parameters);
  return ret;
}


/**
 * @brief Override of `handle_commandline()` which also does `init()`.
 *
 * @return ALL_IS_WELL   if all is well,
 *         EXIT_NO_ERROR if should stop without errors,
 *         EXIT_ERROR    if should stop with errors
 */
CmdParameters::ExitCode CmdParameters::handle_commandline(
  int argc,
  char const *argv[],
  bool show_help_on_error) {

  bool do_continue = true;

  if (init()) {
    do_continue = handle_commandline_intern(argc, argv, show_help_on_error);
  }

  if (has_errors()) {
    if (!m_silent) {
      cout << "Error(s) on command line:\n" << get_errors() << endl;

      if (show_help_on_error) {
        cout << get_usage();
      } else {
        cout << "  Use 'help' or '-h' to view options\n"  << endl;
      }
    }
  }

  if (has_errors()) {
    return EXIT_ERROR;
  } else {
    return (do_continue)?ALL_IS_WELL:EXIT_NO_ERROR;
  }
}


void CmdParameters::show_errors(bool do_explicit) {
  if (has_errors()) {
    if (do_explicit || !m_silent) {
      cout << "Error(s) on command line:\n" << get_errors() << endl;
    }
  } else {
    if (do_explicit) {
      cout << "  No errors encountered\n"  << endl;
    }
  }
}


/**
 * @brief Handle the command line and initialize files/dir's.
 *
 * If help detected, this will return true but there should
 * be no further processing. The caller should check with
 * `has_errors()` if execution can continue.
 *
 * @return true if execution can continue, false otherwise
 */
bool CmdParameters::handle_commandline_intern(
  int argc,
  char const *argv[],
  bool show_help_on_error) {

  //std::cout << "entered handle_commandline_intern()" << std::endl;

  errors.str(""); errors.clear();
  m_p_action = nullptr;
  m_parameters.reset_values();

  // Prescan for help; this overrides everything
  if (handle_help(argc, argv)) return false;

  int curindex = 0;

  try {
    while (true) {
      curindex++;
      if (curindex >= argc) break;

      const char *curarg = argv[curindex];

      //cout << "option: " << curarg << endl;

      // Global options first
      if (process_option(m_parameters, curarg)) continue;

      // Actions
      bool found_action = handle_action(curarg, &errors);
      if (found_action) continue;

      if (m_p_action != nullptr) {
        // Check action options
        if (process_option(m_p_action->parameters, curarg)) {
          //cout << "Found option '" << curarg << "' for action '" << m_p_action->name << "'." << endl;
          found_action = true;
        }
      }
      if (found_action) continue;

      // It's not one an option or an action, so it must be unnamed input
      // NOTE: this implementation check UNNAMED on global options only!
      if (!m_parameters.process_unnamed(curarg)) {
        errors << "  Too many unnamed parameters on command line, '" << curarg << "' unexpected.\n";
      }
    }

    // Check if all unnamed fields have a value
    for (auto &ptr : m_parameters) {
      auto &field = *ptr.get();
      if (field.def_param.param_type != UNNAMED) continue;

      if (field.get_string_value().empty()) {
        errors << "  No " << field.def_param.name << " specified.\n";
      }
    }

    // If actions defined, an action must have been detected
    if (!actions.empty() && m_p_action == nullptr) {
      // Collect all action names
      std::string names;
      for (auto & action : actions) {
        if (!names.empty()) {
          names += " ";
        }

        names += action.name;
      }

      errors << "  Action expected, use one of: " << names << "\n";
    }
  } catch (Exception &e) {
    errors << "  " << e.what() << endl;
  }

  return true;
}


bool CmdParameters::init(CmdParameters const *parent) {
  if (m_done_init) {
    return m_init_result;  // already initialized 
  }

  errors.str(""); errors.clear();
  m_validated = false;

  if (parent != nullptr) {
    if (!add_intern(*parent)) {
      m_init_result = false;
      return false;
    }
  }

  m_init_result = validate()
               && init_params()
               && init_actions();

  m_done_init = true;   // This does not mean that init() was successful!
  return m_init_result;
}


bool CmdParameters::add_intern(CmdParameters const &rhs) {
  // rhs should have been init'ed already (in ctor)

  if (rhs.has_errors()) {
     errors << "Added instance has errors on init()\n"
            <<  rhs.get_errors();
    return false;
  }

  // Add the global param's of the parent to the local list.
  // This means that some double work is done. I don't care
   for(auto &item : rhs.global_parameters) {
    global_parameters.push_back(item);
   }

  // Add parent actions to list of this instance
  // Init on these has already been called
  // TODO: check if this is an issue
   for(auto &action : rhs.actions) {
    actions.push_back(action);
  }

  return true;
}


bool CmdParameters::add(CmdParameters const &rhs) {
  m_done_init = false;  // Need to redo init
  if (!add_intern(rhs)) {
    return false;
  }

  return init();
}


/**
 * @brief Process the usage definitions internally
 *
 * The parameter definitions are converted to an internal representation, better
 * suited for parsing the actual values on the command line.
 *
 * @return true if all went well, false if an error occured during conversion.
 */
bool CmdParameters::init_params() {
  m_parameters.clear();

  for(auto &item : global_parameters) {
    TypedParameter *p = DefParameter_factory(item);
    if (p == nullptr) return false;

    m_parameters.emplace_back(p);
   }
 
  return true;
}


bool CmdParameters::validate() {
  if (m_validated) return true;

  if (m_usage == nullptr) {
    m_validation.add_error("No usage passed");
  }

  m_validated = m_validation.validate(global_parameters, actions);
  errors << m_validation.get_messages(); 

  return m_validated;
}


std::string CmdParameters::get_params(TypedParameter::List const &parameters) const {
  std::string ret;

  bool have_actions = false;
  have_actions = !actions.empty();

  if (have_actions) {
    ret += "\nGlobal Options:\n\n";
  } else {
    ret += "\nOptions:\n\n";
  }

 ret += get_just_params(parameters);

  if (have_actions) {
    ret += "\nNotes:\n\n"
           " * Global options can appear in any position on the command line after the program name.\n"
           " * 'help' combined with an action shows the help for that action.\n"
           " * Actions-specific options must come *after* the action on the commandline.\n";
  }
  ret +=  "\n";

  return ret;
}


std::string CmdParameters::get_just_params(TypedParameter::List const &parameters, bool add_help) const {
  std::string ret;

  StrList disp_defaults;  // TODO: Bring this under TypedParameter::usage()
  StrList disp_params;

  parameters.prepare_usage(disp_defaults, disp_params, add_help);
  unsigned width = max_width(disp_params);

  auto output_padded = [&ret, width, this] (
    TypedParameter &param,
    const string &disp_param,
    const string &disp_default) {
    // Ensure line endings have proper indent
    ret+= "   ";
    ret+= pad(width, disp_param);
    ret+= "  ";
    ret+= set_indent(width + PAD_OFFSET, param.usage());
    ret+= disp_default;
    ret+= "\n";
  };

  int index = 0;

  if (add_help) {
    output_padded(help_switch, disp_params[index], disp_defaults[index]);
    index++;
  }

  for (auto &item: parameters) {
    TypedParameter &param = *item;

    output_padded(param, disp_params[index], disp_defaults[index]);
    index++;
  }

  return ret;
}


/**
 * @brief Scan for help and handle if present.
 *
 * NOTE: Would have been nicer if the CmdParameter for help
 *       could handle this autonomously.
 *
 * @return true if help handled, false otherwise
 */
bool CmdParameters::handle_help(int argc, char const *argv[]) {
  bool have_help = false;
  int curindex = 0;

  while (true) {
    curindex++;
    if (curindex >= argc) break;

    const char *curarg = argv[curindex];
    //std::cout << "curarg:" << curarg << std::endl;

    if (string("help") == curarg || string("-h") == curarg) {
      have_help =true;
      break;
    }
  }

  if (have_help) {
    //std::cout << "handle_help() showing usage" << std::endl;

    if (scan_action(argc, argv)) {
      cout << get_action_usage();
    } else {
      cout << get_usage();
    }
  }

  return have_help;
}


/**
 * @brief determine longest string length in string vector.
 */
unsigned CmdParameters::max_width(StrList const &list) const {
  // Determine max width of displayed param's
  unsigned width = 0;
  for (const string &str : list) {
    // CRAP: Following fails when 'int width = -1', due to signedness
    if (str.length() > width) {
      width = (unsigned) str.length();
    }
  }

  return width;
}


////////////////////////
// Action handling
////////////////////////

bool CmdParameters::init_actions() {
  for(auto &action : actions) {
    if (!action.init_params()) return false;
  }

  return true;
}


bool CmdParameters::handle_action(char const *curarg, Buf *errors) {
  bool found_action = false;

  for (auto &action : actions) {
    if (strcmp(action.name, curarg) == 0) {
      found_action = true;

      if (m_p_action != nullptr) {
        if (errors != nullptr) {
          *errors << "Multiple actions encountered on the command line.\n";
        }
        break;
      }

      m_p_action = &action;
      //cout << "Found action '" << m_p_action->name << "'." << endl;  // DEBUG
    }
  }

  return found_action;
}


bool CmdParameters::scan_action(int argc, char const *argv[]) {
  int curindex = 0;
  m_p_action = nullptr;

  while (true) {
    curindex++;
    if (curindex >= argc) break;

    const char *curarg = argv[curindex];
    if (handle_action(curarg)) return true;
  }

  return false;
}


std::string CmdParameters::get_actions() const {
  std::string ret;

  if (actions.empty()) return ret;

  // Determine max width
  StrList disp_names;
  for (auto &action: actions) {
    disp_names.push_back(action.name);
  }
  unsigned width = max_width(disp_names);

  ret += "\n\nActions:\n\n";

  for (auto &action: actions) {
    ret += "    ";
    ret += pad(width, action.name);
    ret += "   ";
    ret += set_indent(width + PAD_OFFSET, action.usage);
    ret += "\n";
  }

 return ret;
}


std::string CmdParameters::get_action_usage() const {
  std::string ret;

  assert(m_p_action != nullptr);
  auto p = m_p_action;

  ret += "\nHelp for action '";
  ret += p->name;
  ret += "'\n\n";
  ret += "Description:\n\n";

  ret += "  ";
  ret += set_indent(2, p->usage);
  ret += ".\n";

  if (p->long_usage != nullptr) {
    ret += "  ";
    ret += set_indent(2, p->long_usage);
    ret += "\n";
  }

  ret += "\nOptions:\n\n";
  ret += get_just_params(p->parameters, false);
  ret += "\n";

  return ret;
}
