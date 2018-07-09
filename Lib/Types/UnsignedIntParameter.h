#ifndef UNSIGNEDINTPARAMETER_H
#define UNSIGNEDINTPARAMETER_H
#include "IntParameter.h"

struct UnsignedIntParameter : public IntParameter {
public:
  UnsignedIntParameter(DefParameter &var) : IntParameter(var) {}

private:
  bool parse_param_internal(const std::string &in_value) override;
};

#endif // UNSIGNEDINTPARAMETER_H
