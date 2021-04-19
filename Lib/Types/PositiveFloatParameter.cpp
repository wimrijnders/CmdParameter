#include "PositiveFloatParameter.h"
#include <cassert>
#include <string>
#include "Support/Exception.h"
#include "Support/basics.h"
#include "../DefParameter.h"

using std::string;

PositiveFloatParameter::PositiveFloatParameter(DefParameter &var) : TypedParameter(var, "<float>") {
  m_defaults.float_value = var.float_default;

  if (var.float_default <= 0.0f) {
    string msg = "Field '";
    throw Exception(msg + def_param.name + "': default value must be positive.");
  }
}


bool PositiveFloatParameter::parse_param_internal(const std::string &in_value) {
  assert(!in_value.empty());

  float value = get_float_value(in_value);

  if (value <= 0.0f) {
    string msg = "Field '";
    throw Exception(msg + def_param.name + "': value must be positive.");
  }

  m_values.float_value = value;
  m_values.m_detected = true;
  return true;
}


std::string PositiveFloatParameter::default_indicator() const {
  std::string ret;
  ret <<  def_param.float_default;
  return ret;
}
