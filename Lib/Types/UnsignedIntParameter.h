#ifndef UNSIGNEDINTPARAMETER_H
#define UNSIGNEDINTPARAMETER_H
#include "CmdParameter.h"

struct UnsignedIntParameter : public CmdParameter {
public:
  UnsignedIntParameter(DefParameter &var) : CmdParameter(var) {}
  const char *value_indicator() const override { return "<num>"; }

private:
  bool parse_param_internal(const std::string &in_value) override;
};

#endif // UNSIGNEDINTPARAMETER_H
