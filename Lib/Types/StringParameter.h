#ifndef STRINGPARAMETER_H
#define STRINGPARAMETER_H
#include "CmdParameter.h"

struct StringParameter : public CmdParameter {
public:
  StringParameter(DefParameter &var) : CmdParameter(var) {}
  const char *value_indicator() const override { return "<string>"; }
  void default_indicator(std::ostringstream &os) override;

private:
  bool parse_param_internal(const std::string &in_value) override {
    return parse_string_param(in_value);
  }
};

#endif // STRINGPARAMETER_H
