#include "NoneParameter.h"
#include <cassert>
#include <string>

using std::string;

bool NoneParameter::parse_param_internal(const std::string &in_value) {
  assert(in_value.empty());
  m_values.m_detected = true;
  return true;
}
