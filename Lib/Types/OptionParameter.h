#ifndef OPTIONPARAMETER_H
#define OPTIONPARAMETER_H
#include "TypedParameter.h"
#include <vector>


struct OptionParameter : public TypedParameter {
	using Parent  = TypedParameter;
	using Options = std::vector<char const *>;

public:
  OptionParameter(DefParameter &var);
  const char *value_indicator() const override { return "<opt>"; }
  void default_indicator(std::ostringstream &os) override;
	std::string usage() override;

private:
  bool parse_param_internal(const std::string &in_value) override;
  bool set_default() override;
	std::string param_usage_extra();

	Options const *m_options = nullptr;
};

#endif // OPTIONPARAMETER_H
