#ifndef POSITIVEINTPARAMETER_H
#define POSITIVEINTPARAMETER_H
#include "CmdParameter.h"


struct PositiveIntParameter : public CmdParameter {
public:
  PositiveIntParameter(DefParameter &var) : CmdParameter(var) {}
  const char *value_indicator() const override { return "<num>"; }

private:
  bool parse_param_internal(const std::string &in_value) override;
};


#endif // POSITIVEINTPARAMETER_H
