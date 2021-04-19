#ifndef STRINGPARAMETER_H
#define STRINGPARAMETER_H
#include "TypedParameter.h"

struct StringParameter : public TypedParameter {
public:
  StringParameter(DefParameter &var) : TypedParameter(var, "<string>") {}

  std::string default_indicator() const override;

private:
  bool parse_param_internal(const std::string &in_value) override {
    return parse_string_param(in_value);
  }
};

#endif // STRINGPARAMETER_H
