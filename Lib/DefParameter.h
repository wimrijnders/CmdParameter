#ifndef DEFPARAMETER_H
#define DEFPARAMETER_H
#include "Types/ParamType.h"
#include <string>


/**
 * @brief Structure for the definitions of the parameters
 */
struct DefParameter {
  static const int   INT_NOT_SET;
  static const float FLOAT_NOT_SET;

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

  bool         bool_default{false};
  int          int_default{INT_NOT_SET};
  float        float_default{FLOAT_NOT_SET};
  std::string  string_default;

  bool is_int_type() const;
  bool is_float_type() const;

  bool has_default() const;

private:
  void handle_defaults();
};


#endif // DEFPARAMETER_H
