#ifndef INTPARAMETER_H
#define INTPARAMETER_H
#include "../TypedParameter.h"

struct IntParameter : public TypedParameter {
public:
  IntParameter(DefParameter &var) : TypedParameter(var) {}
  const char *value_indicator() const override { return "<num>"; }
  void default_indicator(std::ostringstream &os) override;

private:
  bool parse_param_internal(const std::string &in_value) override;
};

#endif // INTPARAMETER_H
