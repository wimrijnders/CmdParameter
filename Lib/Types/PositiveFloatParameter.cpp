#include "PositiveFloatParameter.h"
#include <cassert>
#include <string>

using std::string;

bool PositiveFloatParameter::parse_param_internal(const std::string &in_value) {
  assert(!in_value.empty());

  float value = get_float_value(in_value);

  if (value <= 0.0f) {
    string msg = "Field '";
    throw string(msg + name + "' value must be positive.");
  }

  float_value = value;
  m_detected = true;
  return true;
}
