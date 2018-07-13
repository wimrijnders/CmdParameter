#include "CmdDefinition.h"
#include <iostream>
#include <sstream>   // ostringstream

using namespace std;

CmdDefinition::CmdDefinition(const char *in_usage, DefParameters global_params) :
  usage(in_usage),
  global_parameters(global_params)
{}


CmdDefinition::CmdDefinition(const char *in_usage, DefActions in_actions) :
usage(in_usage),
actions(in_actions)
{}


CmdDefinition::CmdDefinition(const char *in_usage, DefActions in_actions, DefParameters global_params) :
  usage(in_usage),
  global_parameters(global_params),
  actions(in_actions)
{}


bool CmdDefinition::validate() {
  bool ret = true;

  if (usage == nullptr) {
    cout << "ERROR: No usage passed" << endl;
    ret = false;
  }

  if (global_parameters.empty() && actions.empty()) {
    cout << "WARNING: No global parameters or actions defined. This definition does nothing" << endl;
  }

  // TODO: do this for all actions; also over global and actions as well
  if (!check_labels_distinct(global_parameters)) {
    ret = false;
  }

  if (!check_actions_distinct(actions)) {
    ret = false;
  }

  return ret;
}


/**
 * @brief Check that the parameters are distinct.
 *
 * @return true if unique, false otherwise
 */
bool CmdDefinition::check_labels_distinct(DefParameters &params) {
  bool ret = true;
  int length = (int) params.size();

  if (length == 0 ) {
    int i = 1;  // breakpoint trap
  }

  std::ostringstream msg;

  for (int index1 = 0; index1 < length - 1; ++index1) {
    for (int index2 = index1 + 1; index2 < length; ++index2) {
      // Labels must be  unique
      if (params[index1].name == params[index2].name) {
        msg << "Error: Multiple parameter definitions with name '" << params[index1].name << "'; "
            << "names should be unique\n";
        ret = false;
      }

      // prefixes must be unique
      if (params[index1].prefix == params[index2].prefix) {
        msg << "Error: Multiple parameter definitions with same prefix '" << params[index1].prefix << "': "
            << params[index1].name << " and " << params[index2].name << "\n";
        ret = false;
      }
    }
  }

  if (!ret) {
    std::cout << msg.str() << std::endl;
  }

  return ret;
}


/**
 * @brief Check that the action-labels are unique.
 *
 * @return true if unique, false otherwise
 */
bool CmdDefinition::check_actions_distinct(DefActions &actions) {
  bool ret = true;
  int length = (int) actions.size();

  if (length == 0 ) {
    int i = 1;  // breakpoint trap
  }

  std::ostringstream msg;

  for (int index1 = 0; index1 < length - 1; ++index1) {
    for (int index2 = index1 + 1; index2 < length; ++index2) {
      // Labels must be  unique
      if (actions[index1].name == actions[index2].name) {
        msg << "Error: Multiple actions definitions with name '" << actions[index1].name << "'; "
            << "labels should be unique\n";
        ret = false;
      }
    }
  }

  if (!ret) {
    std::cout << msg.str() << std::endl;
  }

  return ret;
}
