#ifndef INTPARAMETER_H
#define INTPARAMETER_H
#include "../TypedParameter.h"

struct IntParameter : public TypedParameter {
public:
  IntParameter(DefParameter &var);
  std::string default_indicator() const override;

private:
  bool parse_param_internal(const std::string &in_value) override;
};

#endif // INTPARAMETER_H
