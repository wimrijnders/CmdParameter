#ifndef UNNAMEDPARAMETER_H
#define UNNAMEDPARAMETER_H
#include "TypedParameter.h"

struct UnnamedParameter : public TypedParameter {
public:
  UnnamedParameter(DefParameter &var) : TypedParameter(var, "") {}

private:
  bool parse_param_internal(const std::string &in_value) override {
    return parse_string_param(in_value);
  }

	bool takes_value() const override { return false; }
};

#endif // UNNAMEDPARAMETER_H
