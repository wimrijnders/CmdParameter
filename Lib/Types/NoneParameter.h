#ifndef NONEPARAMETER_H
#define NONEPARAMETER_H
#include "../TypedParameter.h"

struct NoneParameter : public TypedParameter {
public:
  NoneParameter(DefParameter &var) : TypedParameter(var, "") {}

private:
  bool parse_param_internal(const std::string &in_value) override;
  bool takes_value() const override { return false; }
};

#endif // NONEPARAMETER_H
