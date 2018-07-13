#ifndef DEFACTION_H
#define DEFACTION_H
#include <vector>
#include "DefParameter.h"


class DefAction {
public:
  DefAction(const char *in_name, const char *in_usage);
  DefAction(
    const char *in_name,
    const char *in_usage,
    std::vector<DefParameter> in_params);

  const char *name;   //> The name of the action.
                      //  This is what is specified on the command line
  const char *usage;  //> Description of the action

  std::vector<DefParameter> params;
};

using DefActions = std::vector<DefAction>;

#endif // DEFACTION_H
