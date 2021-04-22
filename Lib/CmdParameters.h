#ifndef CMDPARAMETERS_H
#define CMDPARAMETERS_H
#include <string>
#include "DefParameter.h"
#include "DefAction.h"
#include "TypedParameter.h"
#include "Types/NoneParameter.h"
#include "CmdValidation.h"


struct CmdParameters {
  using List = TypedParameter::List;
  using Buf  = std::ostringstream;  // because it looks ugly in var declarations

  enum ExitCode {
    ALL_IS_WELL   = -2,
    EXIT_NO_ERROR =  0,
    EXIT_ERROR    =  1
  };

  static NoneParameter help_switch;

  CmdParameters() = default;
  CmdParameters(CmdParameters const &rhs);
  CmdParameters(char const *in_usage, DefParameters global_params, CmdParameters *parent = nullptr);
  CmdParameters(char const *in_usage, DefActions in_actions, CmdParameters *parent = nullptr);

  CmdParameters(
    char const *in_usage,
    DefActions in_actions,
    DefParameters global_params,
    CmdParameters *parent = nullptr);

  bool init(CmdParameters const *parent = nullptr);
  bool add(CmdParameters const &rhs);
  bool validate();
  void silent(bool val) { m_silent = val; }
  std::string get_usage() const;
  std::string params_usage(bool add_help) const;
  TypedParameter::List &parameters() { return m_parameters; }

  void show_errors(bool do_explicit = false);
  std::string get_errors() const { return errors.str(); }
  bool has_errors() const { return !errors.str().empty(); }

  ExitCode handle_commandline(int argc, char const *argv[], bool show_help_on_error = true);

  bool has_help(int argc, char const *argv[]) const;
  bool scan_action(int argc, char const *argv[]);

private:
  const char          *m_usage        = nullptr;
  DefParameters        global_parameters;
  TypedParameter::List m_parameters;
  bool                 m_done_init    = false;
  bool                 m_init_result  = false;
  CmdValidation        m_validation;
  bool                 m_validated    = false;
  DefActions           actions;
  DefAction           *m_p_action     = nullptr;
  Buf                  errors;
  bool                 m_silent       = false;

  static DefParameter help_def;

  void reset();
  bool add_intern(CmdParameters const &rhs);
  bool init_params();
  bool init_actions();
  std::string get_actions() const;
  std::string get_action_usage() const;

  void show_help(int argc, char const *argv[]);
  bool handle_action(char const *curarg, Buf *errors = nullptr);
  bool handle_action_option(char const *curarg);
  void handle_commandline_intern(int argc, char const *argv[]);
  void check_unnamed_fields();
  void check_action_detected();

  std::string get_params(TypedParameter::List const &parameters) const;
  void check_actions_distinct(DefActions &params);
  void output_errors(bool show_help_on_error) const;

  StrList action_names() const;
};

#endif  // CMDPARAMETERS_H
