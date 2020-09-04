#include "PositiveIntParameter.h"
#include "../DefParameter.h"
#include <cassert>
#include <string>
#include "../Support/Exception.h"

using std::string;


PositiveIntParameter::PositiveIntParameter(DefParameter &var) : IntParameter(var) {

  if (m_defaults.int_value <= 0) {
    string msg = "Field '";
    throw Exception(msg + def_param.name + "': default value must be positive.");
  }
}


bool PositiveIntParameter::parse_param_internal(const std::string &in_value) {
  assert(!in_value.empty());

  int value = get_int_value(in_value);

  if (value <= 0) {
    string msg = "Field '";
    throw Exception(msg + def_param.name + "': value must be positive.");
  }

  m_values.int_value = value;
  m_values.m_detected = true;
  return true;
}


