#ifndef OPTIONPARAMETER_H
#define OPTIONPARAMETER_H
#include "TypedParameter.h"
#include <vector>


struct OptionParameter : public TypedParameter {
  using Parent  = TypedParameter;
  using Options = std::vector<char const *>;

public:
  OptionParameter(DefParameter &var);
  std::string usage() const override;

private:
  bool parse_param_internal(const std::string &in_value) override;
  std::string param_usage_extra() const;

  Options const *m_options = nullptr;
};

#endif // OPTIONPARAMETER_H
