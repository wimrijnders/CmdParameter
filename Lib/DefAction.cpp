#include "DefAction.h"


/**
 * @brief Contructor accepting an action name as single parameter.
 *
 * This is a possible situation; an action does not require switched.
 */
DefAction::DefAction(const char *in_name, const char *in_usage) :
  name(in_name),
  usage(in_usage)
{}


DefAction::DefAction(
  const char *in_name,
  const char *in_usage,
  std::vector<DefParameter> in_params) :
  name(in_name),
  usage(in_usage),
  params(in_params)
{}
