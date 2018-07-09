#include "DefParameter.h"


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
  float_default(0.0)
{
}


DefParameter::DefParameter(
  const char *in_name,
  const char *in_prefix,
  ParamType   in_param_type,
  const char *in_usage
) :
  name(in_name),
  prefix(in_prefix),
  param_type(in_param_type),
  usage(in_usage),
  bool_default(false),
  int_default(0),
  float_default(0.0)
{}

DefParameter::DefParameter(
  const char *in_name,
  const char *in_prefix,
  ParamType   in_param_type,
  const char *in_usage,
  int default_value
) :
  name(in_name),
  prefix(in_prefix),
  param_type(in_param_type),
  usage(in_usage),
  bool_default(false),
  int_default(default_value),
  float_default(0.0)
{}


// TODO: check if really needed
DefParameter::DefParameter(
  const char *in_name,
  ParamType   in_param_type,
  const char *in_usage
) :
  name(in_name),
  prefix(nullptr),
  param_type(in_param_type),
  usage(in_usage),
  bool_default(false),
  int_default(0),
  float_default(0.0)
{}

