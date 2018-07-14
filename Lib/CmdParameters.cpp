#include "CmdParameters.h"
#include <iostream>
#include <sstream>   // ostringstream
#include <cstring>   // strcmp
#include "Support/Strings.h"
#include "Types/Types.h"

using namespace std;


// Internal definition of help switch
DefParameter CmdParameters::help_def(
  "Help switch",
  "-h",
  NONE,
  "Show this information. Overrides all other parameters"
);


NoneParameter CmdParameters::help_switch(CmdParameters::help_def);


CmdParameters::CmdParameters(const char *in_usage, DefParameters global_params) :
  usage(in_usage),
  global_parameters(global_params)
{}


/**
 * @brief Create help usage text for all the defined optional param's.
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

	// Prescan for '-h'; this overrides everything
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
			bool found_action = false;
			for (auto &action: actions) {
				if (strcmp(action.name, curarg) == 0) {
					found_action = true;

					if (m_p_action != nullptr) {
						errors << "Multiple actions encountered on the command line.\n";
						break;
					}

					m_p_action = &action;
					// WRI DEBUG
					cout << "Found action '" << m_p_action->name << "'." << endl;
				}
			}
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
			errors << "  Actions defined but none detected on the command line.\n";
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
			cout << "  Use switch '-h' to view options\n"  << endl;
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
  if (!check_labels_distinct(global_parameters)) {
    ret = false;
  }

  m_validated = ret;
  return ret;
}


/**
 * @brief Check that the parameters are distinct.
 *
 * @return true if unique, false otherwise
 */
bool CmdParameters::check_labels_distinct(DefParameters &params) {
  bool ret = true;
  int length = (int) params.size();

  std::ostringstream msg;

  for (int index1 = 0; index1 < length - 1; ++index1) {
    for (int index2 = index1 + 1; index2 < length; ++index2) {
      // Labels must be  unique
      if (params[index1].name == params[index2].name) {
        msg << "Error: Multiple parameter definitions with name '" << params[index1].name << "'; "
            << "names should be unique\n";
        ret = false;
      }

      // prefixes must be unique
      if (params[index1].prefix == params[index2].prefix) {
        msg << "Error: Multiple parameter definitions with same prefix '" << params[index1].prefix << "': "
            << params[index1].name << " and " << params[index2].name << "\n";
        ret = false;
      }
    }
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

  vector<string> disp_defaults;
  vector<string> disp_params;

  parameters.prepare_usage(disp_defaults, disp_params);

  unsigned width = max_width(disp_params);

  if (have_actions) {
    cout << "\nGlobal Options:\n\n";
  } else {
    cout << "\nOptions:\n\n";
  }

  auto output_padded = [width] (
    TypedParameter &param,
    const string &disp_param,
    const string &disp_default) {
    // Ensure line endings have proper indent
    string usage = param.def_param.usage;
    string indent("\n");
    string mt;
    indent += pad(mt, width + PAD_OFFSET);
    usage = Strings::implode(Strings::explode(usage, '\n'), indent.c_str());

    cout << "    " << pad(disp_param, width) << "  " << usage << disp_default << endl;
  };

  int index = 0;
  output_padded(help_switch, disp_params[index], disp_defaults[index]);
  index++;

  for (auto &item: parameters) {
    TypedParameter &param = *item;

    output_padded(param, disp_params[index], disp_defaults[index]);
    index++;
  }

  cout << "\nNotes:\n\n * Global options can appear in any position on the command line after the program name.\n";
  if (have_actions) {
    cout << " * '-h' combined with an action show the help for that action.\n";
    cout << " * Actions-specific options must come *after* the action on the commandline.\n";
  }
  cout << "\n";
}


string CmdParameters::pad(const string &str, unsigned width) {
  string out;

  for (auto i = str.length(); i < width; ++ i) {
    out += " ";
  }

  return str + out;
}


/**
 * @brief Scan for '-h' and handle if present.
 *
 * @return true '-h' handled, false otherwise
 */
bool CmdParameters::handle_help(int argc, const char *argv[]) {
  int curindex = 0;

	while (true) {
		curindex++;
		if (curindex >= argc) break;

		const char *curarg = argv[curindex];

		if (string("-h") == curarg) {
			show_usage();
			return true;
		}
	}

	return false;
}


bool CmdParameters::process_option(List &parameters, const char *curarg) {
  for (auto &item: parameters) {
    TypedParameter &param = *item;

    if (param.parse_param(curarg)) {
      return true;
    }
  }

/*
  // No option detected
  if (Strings::starts_with(curarg, "-")) {
    // Flag anything else that looks like an option param as an error
    throw string("Unknown command line option '") + (curarg + 1) + "'";
  }
*/

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
    if (!action.init_params()) {
      return false;
    }
  }

  return true;
}


void CmdParameters::show_actions() {
  if (actions.empty()) return;

  // Determine max width
  vector<string> disp_names;
  for (auto &action: actions) {
    disp_names.push_back(action.name);
  }
  unsigned width = max_width(disp_names);

  // Adjusted from `output_padded` in `()`
  auto output_padded = [width] (
    const string &label,
    const string &in_usage) {
    // Ensure line endings have proper indent
    string indent("\n");
    string mt;
    indent += pad(mt, width + PAD_OFFSET);
    string usage = Strings::implode(Strings::explode(in_usage, '\n'), indent.c_str());

    cout << "    " << pad(label, width) << "  " << usage << endl;
  };

  cout << "\n\nActions:\n\n";

  for (auto &action: actions) {
    output_padded(action.name, action.usage);
  }
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
