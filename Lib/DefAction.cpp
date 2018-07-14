#include "DefAction.h"
#include <cassert>
#include "Types/Types.h"


/**
 * @brief Copy ctor which explicitly does not copy over `parameters`
 *        This can't be copied due to unique_ptr elements, we don't
 *        want it copied anyway.
 */
DefAction::DefAction(const DefAction &val) :
  name(val.name),
  usage(val.usage),
  params(val.params) {
  assert(val.parameters.empty());
}


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


bool DefAction::init_params() {
  parameters.clear();

  for(auto &item : params) {
    TypedParameter *p = DefParameter_factory(item);
    parameters.emplace_back(p);
  }

  return true;
}
