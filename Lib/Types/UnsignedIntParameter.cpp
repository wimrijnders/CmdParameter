#include "UnsignedIntParameter.h"
#include <cassert>
#include <string>

using std::string;


bool UnsignedIntParameter::parse_param_internal(const std::string &in_value) {
  assert(!in_value.empty());

  int value = get_int_value(in_value);

  if (value < 0) {
    string msg = "Field '";
    throw string(msg + name + "' value must be zero or positive.");
  }

  int_value = value;
  m_detected = true;
  return true;
}
