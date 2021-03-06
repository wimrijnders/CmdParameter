#ifndef DEFPARAMETER_H
#define DEFPARAMETER_H
#include "Types/ParamType.h"
#include <string>
#include <vector>


/**
 * @brief Structure for the definitions of the parameters
 *
 * ------
 * NOTES
 * =====
 *
 * * Passed-in options and prefixes need to be copied internally,
 *   because they can go out of context.
 */
struct DefParameter {
  using Prefixes = std::vector<char const *>;
  using Options = std::vector<char const *>;

  static const int   INT_NOT_SET;
  static const float FLOAT_NOT_SET;

  DefParameter(
    const char *in_name,
    const char *in_prefix,
    ParamType in_param_type,
    const char *in_usage
  );

  DefParameter(
    const char *in_name,
    Prefixes    in_prefixes,
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

  DefParameter(
    const char *in_name,
    Prefixes    in_prefixes,
    ParamType   in_param_type,
    const char *in_usage,
    int default_value
  );

  DefParameter(
    const char *in_name,
    const char *in_prefix,
    Options const &options,  // implies type integer
    const char *in_usage
  );

  DefParameter(
    const char *in_name,
    Prefixes    in_prefixes,
    Options const &options,  // implies type integer
    const char *in_usage
  );

  const char *name;                    //!> The name or short description of the parameter.
                                       //!>  This value is used as label and must be unique
  std::vector<const char *> prefixes;
  ParamType   param_type;              //!> Parameter type
  const char *usage;                   //!> Long description of the parameter, displayed in the help text

  int          int_default   = INT_NOT_SET;
  bool         bool_default  = false;
  float        float_default = FLOAT_NOT_SET;
  std::string  string_default;

  bool is_int_type() const;
  bool is_float_type() const;
  bool has_default() const;
  Options const *options() { return &m_options; }

private:
  void handle_defaults();

  Options const m_options;
};


using DefParameters = std::vector<DefParameter>;

#endif // DEFPARAMETER_H
