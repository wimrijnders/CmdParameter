#ifndef DEFACTION_H
#define DEFACTION_H
#include <vector>
#include "TypedParameter.h"
#include "DefParameter.h"


class DefAction {
public:
  DefAction(const DefAction &val);

  DefAction(
    const char *in_name,
    const char *in_usage,
    const char *in_long_usage);

  DefAction(
    const char *in_name,
    const char *in_usage,
    const char *in_long_usage,
    std::vector<DefParameter> in_params);

  const char *name;        //> The name of the action.
                           //  This is what is specified on the command line
  const char *usage;       //> Short description of the action
  const char *long_usage;  //> Long description of the action

  DefParameters params;
  TypedParameter::List parameters;

  bool init_params();
};

using DefActions = std::vector<DefAction>;

#endif // DEFACTION_H
