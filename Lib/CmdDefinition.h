#ifndef CMDDEFINITION_H
#define CMDDEFINITION_H
#include "DefParameter.h"
#include "DefAction.h"


struct CmdDefinition {
  CmdDefinition(const char *in_usage, DefParameters global_params);
  CmdDefinition(const char *in_usage, DefActions in_actions);
  CmdDefinition(const char *in_usage, DefActions in_actions, DefParameters global_params);

  const char   *usage{nullptr};
  DefParameters global_parameters;
  DefActions    actions;

  bool validate();

private:
  bool check_labels_distinct(DefParameters &params);
  bool check_actions_distinct(DefActions &params);
};

#endif // CMDDEFINITION_H
