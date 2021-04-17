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
  using StrList = std::vector<std::string>;
  using Buf = std::ostringstream;  // because it looks ugly in var declarations

  enum ExitCode {
    ALL_IS_WELL   = -2,
    EXIT_NO_ERROR =  0,
    EXIT_ERROR    =  1
  };

  static NoneParameter help_switch;

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
  void show_errors(bool do_explicit = false);
  std::string get_errors() const { return errors.str(); }
  bool has_errors() const { return !errors.str().empty(); }
  TypedParameter::List &parameters() { return m_parameters; }
  ExitCode handle_commandline(int argc, char const *argv[], bool show_help_on_error = true);

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

  bool add_intern(CmdParameters const &rhs);
  bool init_params();
  bool init_actions();
  bool handle_action(char const *curarg, Buf *errors = nullptr);
  bool scan_action(int argc, char const *argv[]);
  std::string get_actions() const;
  std::string get_action_usage() const;

  bool handle_commandline_intern(int argc, char const *argv[], bool show_help_on_error);
  bool handle_help(int argc, char const *argv[]);
  std::string get_params(TypedParameter::List const &parameters) const;
  std::string get_just_params(TypedParameter::List const &parameters, bool add_help = true) const;
  unsigned max_width(StrList const &list) const;
  void check_actions_distinct(DefActions &params);
};

#endif  // CMDPARAMETERS_H
