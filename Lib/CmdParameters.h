#ifndef CMDPARAMETERS_H
#define CMDPARAMETERS_H
#include <string>
#include "DefParameter.h"
#ifndef LITE
#include "DefAction.h"
#endif  // LITE
#include "TypedParameter.h"
#include "Types/NoneParameter.h"


struct CmdParameters {
	using List = TypedParameter::List;

	enum ExitCode {
		ALL_IS_WELL   = -2,
		EXIT_NO_ERROR =  0,
		EXIT_ERROR    =  1
	};

	CmdParameters(const char *in_usage, DefParameters global_params);

  const char   *usage{nullptr};
  DefParameters global_parameters;

#ifndef LITE
  // Action handling
  CmdParameters(const char *in_usage, DefActions in_actions);
  CmdParameters(const char *in_usage, DefActions in_actions, DefParameters global_params);

  DefActions    actions;

 private:
  bool check_actions_distinct(DefActions &params);
  bool init_actions();
  void show_actions();

 public:

#endif  // LITE
  // TODO: make private
  TypedParameter::List m_parameters;

  bool init_params();
  bool validate();
  void show_usage();
  bool has_errors() const { return m_has_errors; }
  TypedParameter::List &parameters() { return m_parameters; }

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

	bool handle_commandline_intern(
		int argc,
		const char *argv[],
		bool show_help_on_error);

  bool handle_help(int argc, const char *argv[]);
  void show_params(TypedParameter::List &parameters);

  static bool process_option(List &parameters, const char *curarg);
  static std::string pad(const std::string &str, unsigned width);
};

#endif  // CMDPARAMETERS_H
