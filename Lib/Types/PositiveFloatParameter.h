#ifndef POSITIVEFLOATPARAMETER_H
#define POSITIVEFLOATPARAMETER_H
#include "TypedParameter.h"


struct PositiveFloatParameter : public TypedParameter {
public:
  PositiveFloatParameter(DefParameter &var);
  void default_indicator(std::ostringstream &os) override;

private:
  bool parse_param_internal(const std::string &in_value) override;
};

#endif // POSITIVEFLOATPARAMETER_H
