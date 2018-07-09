#include "DefParameter.h"
#include <cassert>
#include <limits>
#include <cmath>


const int   DefParameter::INT_NOT_SET   = std::numeric_limits<int>::min();
const float DefParameter::FLOAT_NOT_SET = nanf("");

/**
 * @brief Contructor accepting null pointer as single parameter
 *
 * Used to indicate and detect the end of a parameterdefinition array.
 */
DefParameter::DefParameter(const char *in_name) :
  name(in_name),
  prefix(nullptr),
  param_type(NONE),
  usage(nullptr),
  bool_default(false),
  int_default(0),
  float_default(0.0) {
  handle_defaults();
}


DefParameter::DefParameter(
  const char *in_name,
  const char *in_prefix,
  ParamType   in_param_type,
  const char *in_usage) :
  name(in_name),
  prefix(in_prefix),
  param_type(in_param_type),
  usage(in_usage) {
  handle_defaults();
}


DefParameter::DefParameter(
  const char *in_name,
  const char *in_prefix,
  ParamType   in_param_type,
  const char *in_usage,
  int default_value) :
  name(in_name),
  prefix(in_prefix),
  param_type(in_param_type),
  usage(in_usage),
  int_default(default_value) {
  handle_defaults();
}


// TODO: check if really needed
DefParameter::DefParameter(
  const char *in_name,
  ParamType   in_param_type,
  const char *in_usage) :
  name(in_name),
  prefix(nullptr),
  param_type(in_param_type),
  usage(in_usage) {
  handle_defaults();
}

bool DefParameter::is_int_type() const {
  switch(param_type) {
  case INTEGER:
  case UNSIGNED_INTEGER:
  case POSITIVE_INTEGER:
    return true;

  default:
    return false;
  }
}

bool DefParameter::is_float_type() const {
  switch(param_type) {
  case POSITIVE_FLOAT:
    return true;

  default:
    return false;
  }
}


void DefParameter::handle_defaults() {
  switch(param_type) {
  case INTEGER:
  case UNSIGNED_INTEGER:
    if (int_default == INT_NOT_SET) {
      int_default = 0;
    }
  break;
  case POSITIVE_INTEGER:
    if (int_default == INT_NOT_SET) {
      int_default = 1;
    }
  break;

  case POSITIVE_FLOAT:
    if (isnan(float_default)) {
      float_default = 0.0f;
    }
  break;

  default:
    // do nothing
    break;
  }
}


bool DefParameter::has_default() const {
  switch(param_type) {
  case INTEGER:
  case UNSIGNED_INTEGER:
  case POSITIVE_INTEGER:
    return (int_default != INT_NOT_SET);

  case POSITIVE_FLOAT:
    return (!isnan(float_default));

  default:
    // Nothing special here
    break;
  }

  return true;
}
