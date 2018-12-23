#ifndef CMDPARAMETERS_H
#define CMDPARAMETERS_H
#include <string>
#include "DefParameter.h"
#ifndef LITE
#include "DefAction.h"
#endif  // LITE
#include "TypedParameter.h"
#include "Types/NoneParameter.h"


class MsgBuffer : public std::ostringstream {
public:
	MsgBuffer() = delete;
	MsgBuffer(MsgBuffer const &rhs) : m_caller(rhs.m_caller), m_error(rhs.m_error) {}
	MsgBuffer(CmdParameters &caller, bool error) : m_caller(caller), m_error(error) {}
	~MsgBuffer();

private:
	CmdParameters &m_caller;
	const bool m_error;
};


struct CmdParameters {
	friend class MsgBuffer;

	using List = TypedParameter::List;
	using StrList = std::vector<std::string>;
	using Buf = std::ostringstream;  // because it looks ugly in var declarations

	enum ExitCode {
		ALL_IS_WELL   = -2,
		EXIT_NO_ERROR =  0,
		EXIT_ERROR    =  1
	};

	CmdParameters(char const *in_usage, DefParameters global_params);

  const char   *usage{nullptr};
  DefParameters global_parameters;

  static NoneParameter help_switch;

#ifndef LITE
  // Action handling
  CmdParameters(char const *in_usage, DefActions in_actions);
  CmdParameters(char const *in_usage, DefActions in_actions, DefParameters global_params);

  DefActions actions;
  DefAction *m_p_action{nullptr};

 private:
  void check_actions_distinct(DefActions &params);
  bool init_actions();
  bool handle_action(char const *curarg, Buf *errors = nullptr);
  bool scan_action(int argc, char const *argv[]);
  void show_actions();
  void show_action_usage();

 public:
#endif  // LITE
  bool init_params();
  bool validate();
  void show_usage();
  bool has_errors() const { return m_has_errors; }
  TypedParameter::List &parameters() { return m_parameters; }

  ExitCode handle_commandline(int argc, char const *argv[], bool show_help_on_error = true);

private:
  enum {
    PAD_OFFSET = 7
  };

  TypedParameter::List m_parameters;
  bool m_has_errors{false};
  bool m_validated{false};
  static DefParameter help_def;

  //
  // Output messages
  //
  StrList m_messages;
  bool m_added_errors{false};
  bool m_added_warnings{false};
  void add_error(std::string const &msg);
  void add_warning(std::string const &msg);
  MsgBuffer add_error() { return MsgBuffer(*this, true); }
  MsgBuffer add_warning() { return MsgBuffer(*this, true); }
  bool output_messages();
  //
  // End output messages
  //

  // Convenience overloads
  void add_error(Buf const &buf) { add_error(buf.str()); }
  void add_warning(Buf const &buf) { add_warning(buf.str()); }

  void check_labels(DefParameters &params);
  void check_parameters(DefParameters &params);
  void check_parameter(DefParameter &param);

  bool handle_commandline_intern(int argc, char const *argv[], bool show_help_on_error);

  bool handle_help(int argc, char const *argv[]);
  void show_params(TypedParameter::List &parameters);
  void show_just_params(TypedParameter::List &parameters, bool add_help = true);
  unsigned max_width(StrList const &list) const;
  std::string set_indent(int indent, std::string const &str);

  static bool process_option(List &parameters, char const *curarg);
  static std::string pad(unsigned width, std::string const &str = "");
};

#endif  // CMDPARAMETERS_H
