#ifndef CMDVALIDATION_H
#define CMDVALIDATION_H
#include <vector>
#include <string>
#include <sstream>   // ostringstream
#include "DefParameter.h"
#include "DefAction.h"

class CmdValidation;


class MsgBuffer : public std::ostringstream {
public:
	MsgBuffer() = delete;
	MsgBuffer(MsgBuffer const &rhs) : m_caller(rhs.m_caller), m_error(rhs.m_error) {}
	MsgBuffer(CmdValidation &caller, bool error) : m_caller(caller), m_error(error) {}
	~MsgBuffer();

private:
	CmdValidation &m_caller;
	bool const m_error;
};


class CmdValidation {
  friend class MsgBuffer;

  using StrList = std::vector<std::string>;
  using Buf = std::ostringstream;  // because it looks ugly in var declarations

public:
  bool validate(DefParameters &params, DefActions &actions);
  void add_error(std::string const &msg);
  void add_warning(std::string const &msg);
  void check_actions_distinct(DefActions &params);
  std::string  get_messages();

private:
  StrList m_messages;
  bool m_added_errors{false};
  bool m_added_warnings{false};
  MsgBuffer add_error() { return MsgBuffer(*this, true); }
  MsgBuffer add_warning() { return MsgBuffer(*this, true); }

  // Convenience overloads
  void add_error(Buf const &buf) { add_error(buf.str()); }
  void add_warning(Buf const &buf) { add_warning(buf.str());
                                   }
  void check_labels(DefParameters &params);
  void check_parameters(DefParameters &params);
  void check_parameter(DefParameter &param);
};

#endif  // CMDVALIDATION_H
