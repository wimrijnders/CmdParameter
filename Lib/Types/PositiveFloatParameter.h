#ifndef POSITIVEFLOATPARAMETER_H
#define POSITIVEFLOATPARAMETER_H
#include "CmdParameter.h"


struct PositiveFloatParameter : public CmdParameter {
public:
  PositiveFloatParameter(DefParameter &var) : CmdParameter(var) {}
  const char *value_indicator() const override { return "<float>"; }

private:
  bool parse_param_internal(const std::string &in_value) override;
};

#endif // POSITIVEFLOATPARAMETER_H
