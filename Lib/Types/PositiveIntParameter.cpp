#include "PositiveIntParameter.h"
#include "../DefParameter.h"
#include <cassert>
#include <string>

using std::string;


bool PositiveIntParameter::parse_param_internal(const std::string &in_value) {
  assert(!in_value.empty());

  int value = get_int_value(in_value);

  if (value <= 0) {
    string msg = "Field '";
    throw string(msg + def_param.name + "' value must be positive.");
  }

  int_value = value;
  m_detected = true;
  return true;
}


