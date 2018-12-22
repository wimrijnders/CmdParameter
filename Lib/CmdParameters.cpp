#include "CmdParameters.h"
#include <cassert>
#include <iostream>
#include <sstream>   // ostringstream
#include <cstring>   // strcmp
#include "Support/Strings.h"
#include "Types/Types.h"

using namespace std;


// Internal definition of help switch
DefParameter CmdParameters::help_def(
  "Help switch",
  "help", "-h",
  NONE,
  "Show this information. Overrides all other parameters"
);


NoneParameter CmdParameters::help_switch(CmdParameters::help_def);


CmdParameters::CmdParameters(const char *in_usage, DefParameters global_params) :
  usage(in_usage),
  global_parameters(global_params)
{}


/**
 * @brief Create help usage text for all the defined global parameters.
 *
 * Output per option has the form:
 *
 *   switch[=<param_type>] description[<text for default>]
 *
 * Some effort is done to keep the descriptions aligned.
 */
void CmdParameters::show_usage() {
  cout << usage;
#ifndef LITE
  show_actions();
#endif  // LITE
  show_params(m_parameters);
}


/**
 * @brief Override of `handle_commandline()` which also does `init_params()`.
 *
 * @return ALL_IS_WELL   if all is well,
 *         EXIT_NO_ERROR if should stop without errors,
 *         EXIT_ERROR    if should stop with errors
 */
CmdParameters::ExitCode CmdParameters::handle_commandline(
  int argc,
  const char* argv[],
  bool show_help_on_error) {
  if (!init_params()) return EXIT_ERROR;

  if (!handle_commandline_intern(argc, argv, show_help_on_error)) {
    if(has_errors()) {
      return EXIT_ERROR;
    } else {
      return EXIT_NO_ERROR;
    }
  }

  return ALL_IS_WELL;
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
	const char *argv[],
	bool show_help_on_error) {
	ostringstream errors;

	m_has_errors = false;
#ifndef LITE
	m_p_action = nullptr;
#endif  // LITE

	// Prescan for help; this overrides everything
	if (handle_help(argc, argv)) return false;

	int curindex = 0;

	try {
		while (true) {
			curindex++;
			if (curindex >= argc) break;

			const char *curarg = argv[curindex];

			// Global options first
			if (process_option(m_parameters, curarg)) continue;
#ifndef LITE

			// Actions
			bool found_action = handle_action(curarg, &errors);
			if (found_action) continue;

			if (m_p_action != nullptr) {
				// Check action options
				if (process_option(m_p_action->parameters, curarg)) {
					// WRI DEBUG
					cout << "Found option '" << curarg << "' for action '" << m_p_action->name << "'." << endl;
					found_action = true;
				}
			}
			if (found_action) continue;

			// It's not one an option or an action, so it must be unnamed input
			// NOTE: this implementation check UNNAMED on global options only!
			if (!m_parameters.process_unnamed(curarg)) {
				errors << "  Too many unnamed parameters on command line, '" << curarg << " unexpected.\n";
			}
#else  // LITE
			errors << "  Unknown parameter '" << curarg << "'.\n";
#endif  // LITE
		}
#ifndef LITE

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
#endif  // LITE
	} catch (string &error) {
		if (error != "all is well") {
			errors << "  " << error.c_str() << endl;
		}
	}

	string err_string = errors.str();

	if (!err_string.empty()) {
		cout << "Error(s) on command line:\n" << err_string.c_str() << endl;

		if (show_help_on_error) {
			show_usage();
		} else {
			cout << "  Use 'help' or '-h' to view options\n"  << endl;
		}

		m_has_errors = true;
	}

	return !m_has_errors;
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
  if (!validate()) return false;

  m_parameters.clear();

  for(auto &item : global_parameters) {
    TypedParameter *p = DefParameter_factory(item);
    m_parameters.emplace_back(p);
  }
#ifndef LITE

  if (!init_actions()) return false;
#endif  // LITE
  return true;
}


bool CmdParameters::validate() {
  if (m_validated) return true;
  bool ret = true;

  if (usage == nullptr) {
    cout << "ERROR: No usage passed" << endl;
    ret = false;
  }

#ifndef LITE
  if (global_parameters.empty() && actions.empty()) {
    cout << "WARNING: No global parameters or actions defined. This definition does nothing" << endl;
  }

  if (!check_actions_distinct(actions)) {
    ret = false;
  }

  // TODO: do this for all actions; also over global and actions as well
#else  // LITE
  if (global_parameters.empty()) {
    cout << "WARNING: No global parameters defined. This definition does nothing" << endl;
  }

#endif  // LITE
  if (!check_parameters(global_parameters)) {
    ret = false;
  }

  if (!check_labels(global_parameters)) {
    ret = false;
  }

  m_validated = ret;
  return ret;
}


bool CmdParameters::check_labels(DefParameters &params) {
  bool ret = true;
  int length = (int) params.size();
  std::ostringstream msg;

  // No empty names
  int index = 0;
  for (auto &p : params) {
    if (p.name == nullptr) {
      msg << "Error: name can not be a null pointer "
          << "for parameter with index " << index << "\n";
      ret = false;
      continue; // protect for next check
    }

    if (p.name[0] == '\0') {
      msg << "Error: name can not be an empty string "
          << "for parameter with index " << index << "\n";
      ret = false;
    }

    index++;
  }


  for (int index1 = 0; index1 < length - 1; ++index1) {
    for (int index2 = index1 + 1; index2 < length; ++index2) {
      auto &p1 = params[index1];
      auto &p2 = params[index2];

      // Labels must be unique
      if (!strcmp(p1.name, p2.name)) {
        msg << "Error: Multiple parameter definitions with name '" << p1.name << "'; "
            << "names should be unique\n";
        ret = false;
      }

      // prefixes must be unique
      for (auto &pr1 : p1.prefixes) {
        for (auto &pr2 : p2.prefixes) {
          // Skip empties, these have been tested
          // beforehand in check_parameters()
          if (pr1[0] == '\0') continue;
          if (pr2[0] == '\0') continue;

          if (!strcmp(pr1, pr2)) {
            msg << "Error: Multiple parameter definitions with same prefix '" << pr1 << "' "
                << "for " << p1.name << " and " << p2.name << "\n";
            ret = false;
          }
        }
      }
    }
  }

  if (!ret) {
    std::cout << msg.str() << std::endl;
  }

  return ret;
}


bool CmdParameters::check_parameters(DefParameters &params) {
  bool ret = true;

  for (auto &p : params) {
    if (!check_parameter(p)) {
        ret = false;
    }
  }


  return ret;
}


bool CmdParameters::check_parameter(DefParameter &param) {
  auto &p = param;  // Local aliase
  bool ret = true;
  std::ostringstream msg;

  // At least one prefix present
  if (p.prefixes.empty()) {
    msg << "Error: At least one prefix must be defined "
        << "in parameter " << p.name << "\n";
    ret = false;
  }

  // no empty prefix strings
  for (auto &pr : p.prefixes) {
    if (pr[0] == '\0') {
      msg << "Error: prefixes can not be an empty string "
          << "in parameter " << p.name << "\n";
      ret = false;
    }
  }

  // prefixes must be unique
  if (p.prefixes.size() >= 2) {
    for (int i = 0; i < (int) p.prefixes.size() -1; ++i) {
      for (int j = i + 1; j < (int) p.prefixes.size(); ++j) {
        auto &pr1 = p.prefixes[i];
        auto &pr2 = p.prefixes[j];

        if (strcmp(pr1, pr2)) {
          msg << "Error: Duplicate prefixes '" << pr1 << "' "
              << "for " << p.name << "\n";
          ret = false;
        }
      }
    }
  }

  // Long text must be present
  if (p.usage == nullptr || p.usage[0] == '\0') {
    msg << "Error: Empty long text passed "
        << "in parameter '" << p.name << "'; this field is required\n";
    ret = false;
  }

  if (!ret) {
    std::cout << msg.str() << std::endl;
  }

  return ret;
}


void CmdParameters::show_params(TypedParameter::List &parameters) {
  bool have_actions = false;
#ifndef LITE
  have_actions = !actions.empty();
#endif  // LITE

  if (have_actions) {
    cout << "\nGlobal Options:\n\n";
  } else {
    cout << "\nOptions:\n\n";
  }

 show_just_params(parameters);

  cout << "\nNotes:\n\n * Global options can appear in any position on the command line after the program name.\n";
  if (have_actions) {
    cout << " * 'help' combined with an action shows the help for that action.\n";
    cout << " * Actions-specific options must come *after* the action on the commandline.\n";
  }
  cout << "\n";
}


void CmdParameters::show_just_params(TypedParameter::List &parameters, bool add_help) {
  vector<string> disp_defaults;
  vector<string> disp_params;

  parameters.prepare_usage(disp_defaults, disp_params, add_help);
  unsigned width = max_width(disp_params);

  auto output_padded = [width, this] (
    TypedParameter &param,
    const string &disp_param,
    const string &disp_default) {
    // Ensure line endings have proper indent
    cout << "    " << pad(disp_param, width)
         << "  " << this->set_indent(width + PAD_OFFSET, param.def_param.usage)
         << disp_default << endl;
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
}


string CmdParameters::pad(const string &str, unsigned width) {
  string out;

  for (auto i = str.length(); i < width; ++ i) {
    out += " ";
  }

  return str + out;
}


/**
 * @brief Scan for help and handle if present.
 *
 * NOTE: Would have been nicer if the CmdParameter for help
 *       could handle this autonomously.
 *
 * @return true if help handled, false otherwise
 */
bool CmdParameters::handle_help(int argc, const char *argv[]) {
  bool have_help = false;
  int curindex = 0;

	while (true) {
		curindex++;
		if (curindex >= argc) break;

		const char *curarg = argv[curindex];

		if (string("help") == curarg || string("-h") == curarg) {
			have_help =true;
			break;
		}
	}

	if (have_help) {
#ifndef LITE
		if (scan_action(argc, argv)) {
			show_action_usage();
		} else {
			show_usage();
		}
#else  // LITE
		show_usage();
#endif  // LITE
	}

	return have_help;
}


bool CmdParameters::process_option(List &parameters, const char *curarg) {
  for (auto &item: parameters) {
    TypedParameter &param = *item;
    if (param.parse_param(curarg)) return true;
  }

  return false;
}


/**
 * @brief determine longest string length in string vector.
 */
unsigned CmdParameters::max_width(const std::vector<std::string> &list) const {
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


string CmdParameters::set_indent(int indent, const string &str) {
  string str_indent("\n");
  string mt;
  str_indent += pad(mt, indent);
  return Strings::implode(Strings::explode(str, '\n'), str_indent.c_str());
}
#ifndef LITE


////////////////////////
// Action handling
////////////////////////

CmdParameters::CmdParameters(const char *in_usage, DefActions in_actions) :
  usage(in_usage),
  actions(in_actions)
{}


CmdParameters::CmdParameters(const char *in_usage, DefActions in_actions, DefParameters global_params) :
  usage(in_usage),
  global_parameters(global_params),
  actions(in_actions)
{}


bool CmdParameters::init_actions() {
  for(auto &action : actions) {
    if (!action.init_params()) return false;
  }

  return true;
}


bool CmdParameters::handle_action(const char *curarg, ostringstream *errors) {
	bool found_action = false;

	for (auto &action: actions) {
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


bool CmdParameters::scan_action(int argc, const char *argv[]) {
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


void CmdParameters::show_actions() {
  if (actions.empty()) return;

  // Determine max width
  vector<string> disp_names;
  for (auto &action: actions) {
    disp_names.push_back(action.name);
  }
  unsigned width = max_width(disp_names);

  cout << "\n\nActions:\n\n";

  for (auto &action: actions) {
    cout << "    " << pad(action.name, width)
         << "  " << set_indent(width + PAD_OFFSET, action.usage) << endl;
  }
}


void CmdParameters::show_action_usage() {
  assert(m_p_action != nullptr);
  auto p = m_p_action;

  cout << "\nHelp for action '" << p->name << "'\n\n"
          "Description:\n\n";

  cout << "  " << set_indent(2, p->usage) << ".\n";

  if (p->long_usage != nullptr) {
    cout << "  " << set_indent(2, p->long_usage) << "\n";
  }

  cout << "\nOptions:\n\n";
  show_just_params(p->parameters, false);
  cout << "\n";
}


/**
 * @brief Check that the action-labels are unique.
 *
 * @return true if unique, false otherwise
 */
bool CmdParameters::check_actions_distinct(DefActions &actions) {
  bool ret = true;
  int length = (int) actions.size();
  std::ostringstream msg;

  for (int index1 = 0; index1 < length - 1; ++index1) {
    for (int index2 = index1 + 1; index2 < length; ++index2) {
      // Labels must be  unique
      if (actions[index1].name == actions[index2].name) {
        msg << "Error: Multiple actions definitions with name '" << actions[index1].name << "'; "
            << "labels should be unique\n";
        ret = false;
      }
    }
  }

  if (!ret) {
    std::cout << msg.str() << std::endl;
  }

  return ret;
}
#endif  // LITE
