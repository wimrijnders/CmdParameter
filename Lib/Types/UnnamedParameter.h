#ifndef UNNAMEDPARAMETER_H
#define UNNAMEDPARAMETER_H
#include "CmdParameter.h"

struct UnnamedParameter : public CmdParameter {
public:
  UnnamedParameter(DefParameter &var) : CmdParameter(var) {}
  const char *value_indicator() const override { return ""; }

private:
  bool parse_param_internal(const std::string &in_value) override {
    return parse_string_param(in_value);
  }
};

#endif // UNNAMEDPARAMETER_H
