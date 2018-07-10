#ifndef DEFPARAMETER_H
#define DEFPARAMETER_H
#include "Types/ParamType.h"
#include <string>


/**
 * @brief Structure for the definitions of the parameters
 */
struct DefParameter {
  static const int   INT_NOT_SET;
#ifndef LITE
  static const float FLOAT_NOT_SET;
#endif  // LITE

  DefParameter(const char *in_name);

  DefParameter(
    const char *in_name,
    const char *in_prefix,
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

#ifndef LITE
#if 0
  DefParameter(
    const char *in_name,
    ParamType in_param_type,
    const char *in_usage
  );
#endif

#endif  // LITE
  const char *name;       //> The name or short description of the parameter.
                          //  This value is used as label and must be unique
  const char *prefix;
  ParamType   param_type; //> Parameter type
  const char *usage;      //> Long description of the parameter, displayed in the help text

  int          int_default{INT_NOT_SET};

  bool is_int_type() const;
#ifndef LITE

  bool         bool_default{false};
  float        float_default{FLOAT_NOT_SET};
  std::string  string_default;

  bool is_float_type() const;

#endif  // LITE
  bool has_default() const;

private:
  void handle_defaults();
};


#endif // DEFPARAMETER_H
