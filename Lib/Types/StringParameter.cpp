#include "StringParameter.h"
#include "../DefParameter.h"
#include "Support/basics.h"


std::string StringParameter::default_indicator() const {
  return def_param.string_default;
}
