#ifndef DEFPARAMETER_H
#define DEFPARAMETER_H
#include "ParamType.h"
#include <string>


/**
 * @brief Structure for the definitions of the parameters
 */
struct DefParameter {
  DefParameter(const char *in_name);

  DefParameter(
    const char *in_name,
    const char *in_prefix,
    ParamType in_param_type,
    const char *in_usage
  );

  DefParameter(
    const char *in_name,
    ParamType in_param_type,
    const char *in_usage
  );

  DefParameter(
    const char *in_name,
    const char *in_prefix,
    ParamType   in_param_type,
    const char *in_usage,
    int default_value
  );

  const char *name;       //> The name or short description of the parameter.
                          //  This value is used as label and must be unique
  const char *prefix;
  ParamType   param_type; //> Parameter type
  const char *usage;      //> Long description of the parameter, displayed in the help text

  bool         bool_default;
  int          int_default;
  float        float_default;
  std::string  string_default;
};


#endif // DEFPARAMETER_H
