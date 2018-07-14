#include "DefParameter.h"
#include <cassert>
#include <limits>
#include <cmath>


const int   DefParameter::INT_NOT_SET   = std::numeric_limits<int>::min();
#ifndef LITE
const float DefParameter::FLOAT_NOT_SET = nanf("");
#endif  // LITE


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


bool DefParameter::is_int_type() const {
  switch(param_type) {
  case INTEGER:
#ifndef LITE
  case UNSIGNED_INTEGER:
  case POSITIVE_INTEGER:
#endif  // LITE
    return true;

  default:
    return false;
  }
}


#ifndef LITE
bool DefParameter::is_float_type() const {
  switch(param_type) {
  case POSITIVE_FLOAT:
    return true;

  default:
    return false;
  }
}


#endif  // LITE
void DefParameter::handle_defaults() {
  switch(param_type) {
  case INTEGER:
    if (int_default == INT_NOT_SET) {
      int_default = 0;
    }
  break;
#ifndef LITE
  case UNSIGNED_INTEGER:
    if (int_default == INT_NOT_SET) {  // Note: same as INTEGER
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

#endif  // LITE
  default:
    // do nothing
    break;
  }
}


bool DefParameter::has_default() const {
  switch(param_type) {
  case INTEGER:
    return (int_default != INT_NOT_SET);

#ifndef LITE
  case UNSIGNED_INTEGER:
  case POSITIVE_INTEGER:
    return (int_default != INT_NOT_SET);  // Note: same as INTEGER

  case POSITIVE_FLOAT:
    return (!isnan(float_default));

#endif  // LITE
  default:
    // Nothing special here
    break;
  }

  return true;
}
