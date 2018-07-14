#include "PositiveFloatParameter.h"
#include <cassert>
#include <string>
#include <sstream>
#include "../DefParameter.h"

using std::string;

bool PositiveFloatParameter::parse_param_internal(const std::string &in_value) {
  assert(!in_value.empty());

  float value = get_float_value(in_value);

  if (value <= 0.0f) {
    string msg = "Field '";
    throw string(msg + def_param.name + "' value must be positive.");
  }

  float_value = value;
  m_detected = true;
  return true;
}


void PositiveFloatParameter::default_indicator(std::ostringstream &os) {
  os << def_param.float_default;
}
