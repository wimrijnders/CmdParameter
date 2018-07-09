#ifndef POSITIVEINTPARAMETER_H
#define POSITIVEINTPARAMETER_H
#include "IntParameter.h"


struct PositiveIntParameter : public IntParameter {
public:
  PositiveIntParameter(DefParameter &var) : IntParameter(var) {}

private:
  bool parse_param_internal(const std::string &in_value) override;
};


#endif // POSITIVEINTPARAMETER_H
