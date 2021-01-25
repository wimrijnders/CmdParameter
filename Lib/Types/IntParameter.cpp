#include "IntParameter.h"
#include <cassert>
#include <string>
#include <sstream>
#include "../DefParameter.h"
#include "Support/debug.h"


IntParameter::IntParameter(DefParameter &var) : TypedParameter(var, "<num>") {
  m_defaults.int_value = var.int_default;
}


bool IntParameter::parse_param_internal(const std::string &in_value) {
  assert(!in_value.empty());

  int value = get_int_value(in_value);

  m_values.int_value = value;
  m_values.m_detected = true;
  return true;
}


void IntParameter::default_indicator(std::ostringstream &os) {
  os << def_param.int_default;
}
