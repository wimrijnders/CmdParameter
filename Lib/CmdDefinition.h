#ifndef CMDDEFINITION_H
#define CMDDEFINITION_H
#include <string>
#include "DefParameter.h"
#include "DefAction.h"
#include "CmdParameter.h"
#include "Types/NoneParameter.h"


struct CmdDefinition {
	using List = CmdParameter::List;

	enum ExitCode {
		ALL_IS_WELL   = -2,
		EXIT_NO_ERROR =  0,
		EXIT_ERROR    =  1
	};

  CmdDefinition(const char *in_usage, DefParameters global_params);
  CmdDefinition(const char *in_usage, DefActions in_actions);
  CmdDefinition(const char *in_usage, DefActions in_actions, DefParameters global_params);

  const char   *usage{nullptr};
  DefParameters global_parameters;
  DefActions    actions;

  // TODO: make private
  CmdParameter::List m_parameters;

  bool init_params();
  bool validate();
  void show_usage();
  bool has_errors() const { return m_has_errors; }
  CmdParameter::List &parameters() { return m_parameters; }

  ExitCode handle_commandline(
    int argc,
    const char *argv[],
    bool show_help_on_error = true);

  // TODO private
  static NoneParameter help_switch;

private:
  bool m_has_errors{false};
  bool m_validated{false};
  static DefParameter help_def;

  bool check_labels_distinct(DefParameters &params);
  bool check_actions_distinct(DefActions &params);

	bool handle_commandline_intern(
		int argc,
		const char *argv[],
		bool show_help_on_error);

  bool handle_help(int argc, const char *argv[]);
  void show_params(CmdParameter::List &parameters);

  static bool process_option(List &parameters, const char *curarg);
  static std::string pad(const std::string &str, unsigned width);
};

#endif // CMDDEFINITION_H
