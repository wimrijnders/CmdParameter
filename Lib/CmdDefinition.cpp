#include "CmdDefinition.h"
#include <iostream>
#include <sstream>   // ostringstream
#include "Support/Strings.h"
#include "Types/Types.h"

using namespace std;


// Internal definition of help switch
DefParameter CmdDefinition::help_def(
  "Help switch",
  "-h",
  NONE,
  "Show this information. Overrides all other parameters"
);


NoneParameter CmdDefinition::help_switch(CmdDefinition::help_def);


CmdDefinition::CmdDefinition(const char *in_usage, DefParameters global_params) :
  usage(in_usage),
  global_parameters(global_params)
{}


CmdDefinition::CmdDefinition(const char *in_usage, DefActions in_actions) :
  usage(in_usage),
  actions(in_actions)
{}


CmdDefinition::CmdDefinition(const char *in_usage, DefActions in_actions, DefParameters global_params) :
  usage(in_usage),
  global_parameters(global_params),
  actions(in_actions)
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
void CmdDefinition::show_usage() {
  cout << usage;

  if (!actions.empty()) {
    cout << "\n\nActions:\n\n";

    for (auto &action: actions) {
      // TODO: padding between name and usage
      cout << "    " << action.name << "   " << action.usage << "\n";
    }

  }

  show_params(m_parameters);
}


/**
 * @brief Override of `handle_commandline()` which also does `init_params()`.
 *
 * @return ALL_IS_WELL   if all is well,
 *         EXIT_NO_ERROR if should stop without errors,
 *         EXIT_ERROR    if should stop with errors
 */
CmdDefinition::ExitCode CmdDefinition::handle_commandline(
  int argc,
  const char* argv[],
  bool show_help_on_error) {
  if (!init_params()) {
    return EXIT_ERROR;
  }

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
bool CmdDefinition::handle_commandline_intern(
	int argc,
	const char *argv[],
	bool show_help_on_error) {
	ostringstream errors;

	m_has_errors = false;

	// Prescan for '-h'; this overrides everything
	if (handle_help(argc, argv)) return false;

	int curindex = 0;

	try {
		while (true) {
			curindex++;
			if (curindex >= argc) break;

			const char *curarg = argv[curindex];

			if (!process_option(m_parameters, curarg)) {
#ifndef LITE
				// It's not one of the defined options, so it must be unnamed input
				if (!m_parameters.process_unnamed(curarg)) {
					errors << "  Too many unnamed parameters on command line, '" << curarg << " unexpected.\n";
				}
#else  // LITE
				errors << "  Unknown parameter '" << curarg << "'.\n";
#endif  // LITE
			}
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
bool CmdDefinition::init_params() {
  if (!validate()) return false;

  m_parameters.clear();

  for(auto &item : global_parameters) {
    CmdParameter *p = DefParameter_factory(item);
    m_parameters.emplace_back(p);
  }

  for(auto &action : actions) {
    if (!action.init_params()) {
      return false;
    }
  }

  return true;
}


bool CmdDefinition::validate() {
  if (m_validated) return true;
  bool ret = true;

  if (usage == nullptr) {
    cout << "ERROR: No usage passed" << endl;
    ret = false;
  }

  if (global_parameters.empty() && actions.empty()) {
    cout << "WARNING: No global parameters or actions defined. This definition does nothing" << endl;
  }

  // TODO: do this for all actions; also over global and actions as well
  if (!check_labels_distinct(global_parameters)) {
    ret = false;
  }

  if (!check_actions_distinct(actions)) {
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
bool CmdDefinition::check_labels_distinct(DefParameters &params) {
  bool ret = true;
  int length = (int) params.size();

  if (length == 0 ) {
    int i = 1;  // breakpoint trap
  }

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


/**
 * @brief Check that the action-labels are unique.
 *
 * @return true if unique, false otherwise
 */
bool CmdDefinition::check_actions_distinct(DefActions &actions) {
  bool ret = true;
  int length = (int) actions.size();

  if (length == 0 ) {
    int i = 1;  // breakpoint trap
  }

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


void CmdDefinition::show_params(CmdParameter::List &parameters) {
  vector<string> disp_defaults;
  vector<string> disp_params;

  parameters.prepare_usage(disp_defaults, disp_params);

  // Determine max width of displayed param's
  unsigned width = 0;
  for (unsigned i = 0; i < disp_params.size(); ++i) {
    string &str = disp_params[i];
    // CRAP: Following fails when 'int width = -1', due to signedness
    if (str.length() > width) {
      width = (unsigned) str.length();
    }
  }

  if (actions.empty()) {
    cout << "\nOptions:\n";
  } else {
    cout << "\nGlobal Options:\n";
  }
  cout << "    (Can appear in any position on the command line after the program name)\n";


  auto output_padded = [width] (
    CmdParameter &param,
    const string &disp_param,
    const string &disp_default) {
    // Ensure line endings have proper indent
    string usage = param.def_param.usage;
    string indent("\n");
    string mt;
    indent += pad(mt, width + 4 + 3);
    usage = Strings::implode(Strings::explode(usage, '\n'), indent.c_str());

    cout << "    " << pad(disp_param, width) << "  " << usage << disp_default << endl;
  };

  int index = 0;
  output_padded(help_switch, disp_params[index], disp_defaults[index]);
  index++;

  for (auto &item: parameters) {
    CmdParameter &param = *item;

    output_padded(param, disp_params[index], disp_defaults[index]);
    index++;
  }
}


string CmdDefinition::pad(const string &str, unsigned width) {
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
bool CmdDefinition::handle_help(int argc, const char *argv[]) {
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


bool CmdDefinition::process_option(List &parameters, const char *curarg) {
  for (auto &item: parameters) {
    CmdParameter &param = *item;

    if (param.parse_param(curarg)) {
      return true;
    }
  }

  // No option detected
  if (Strings::starts_with(curarg, "-")) {
    // Flag anything else that looks like an option param as an error
    throw string("Unknown command line option '") + (curarg + 1) + "'";
  }

  return false;
}
