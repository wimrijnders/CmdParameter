#include "CmdValidation.h"
#include <cassert>
#include <iostream>
#include <cstring>   // strcmp
#include "DefParameter.h"

using namespace std;


MsgBuffer::~MsgBuffer() {
  auto output = str();
  if (output.empty()) return;

  if (m_error) {
    m_caller.add_error(output);
  } else {
    m_caller.add_warning(output);
  }
}


//////////////////////////////////////////////////////////////

namespace {
  string const ERROR_PREFIX = "ERROR: ";
  string const WARN_PREFIX  = "WARNING: ";
}

void CmdValidation::add_error(const std::string &msg) {
  m_messages.push_back(ERROR_PREFIX + msg);
  m_added_errors = true;
}


void CmdValidation::add_warning(const std::string &msg) {
  m_messages.push_back(WARN_PREFIX + msg);
  m_added_warnings = true;
}


std::string CmdValidation::get_messages() {
  assert((m_added_errors || m_added_warnings) == !m_messages.empty());
	Buf out;

  if (!m_messages.empty()) {
    out << "Messages during validation:\n";

    for (auto const &msg :m_messages) {
      out << "  " << msg << "\n";
    }

    out << endl;
  }

  return out.str();
}


bool CmdValidation::validate(DefParameters &params, DefActions &actions) {
  if (params.empty() && actions.empty()) {
    add_warning("WARNING: No global parameters or actions defined. This definition does nothing");
  }

  check_actions_distinct(actions);

  check_labels(params);
  check_parameters(params);
  return !m_added_errors;
}


void CmdValidation::check_labels(DefParameters &params) {
  int length = (int) params.size();

  // No empty names
  int index = 0;
  for (auto &p : params) {
    if (p.name == nullptr) {
      add_error() << "Name can not be a null pointer "
                  << "for parameter with index " << index;
      continue; // protect for next check
    }

    if (p.name[0] == '\0') {
      add_error() << "Name can not be an empty string "
                  << "for parameter with index " << index;
    }

    index++;
  }


  for (int index1 = 0; index1 < length - 1; ++index1) {
    for (int index2 = index1 + 1; index2 < length; ++index2) {
      auto &p1 = params[index1];
      auto &p2 = params[index2];

      // Labels must be unique
      if (!strcmp(p1.name, p2.name)) {
        add_error() << "Multiple parameter definitions with name '" << p1.name << "'; "
                    << "names should be unique";
      }

      // prefixes must be unique
      for (auto &pr1 : p1.prefixes) {
        for (auto &pr2 : p2.prefixes) {
          // Skip empties, these have been tested
          // beforehand in check_parameters()
          if (pr1[0] == '\0') continue;
          if (pr2[0] == '\0') continue;

          if (!strcmp(pr1, pr2)) {
            add_error() << "Multiple parameter definitions with same prefix '" << pr1 << "', "
                        << "namely parameters '" << p1.name << "' and '" << p2.name << "'";
          }
        }
      }
    }
  }
}


void CmdValidation::check_parameters(DefParameters &params) {
  for (auto &p : params) {
    check_parameter(p);
  }
}


void CmdValidation::check_parameter(DefParameter &param) {
  auto &p = param;  // Local alias

  // At least one prefix present
  if (p.prefixes.empty()) {
    add_error() << "At least one prefix must be defined "
                << "in parameter " << p.name;
  }

  // no empty prefix strings
  for (auto &pr : p.prefixes) {
    if (pr[0] == '\0') {
      add_error() << "prefixes can not be an empty string "
                  << "in parameter " << p.name;
    }
  }

  // prefixes must be unique
  if (p.prefixes.size() >= 2) {
    for (int i = 0; i < (int) p.prefixes.size() -1; ++i) {
      for (int j = i + 1; j < (int) p.prefixes.size(); ++j) {
        auto &pr1 = p.prefixes[i];
        auto &pr2 = p.prefixes[j];

        if (strcmp(pr1, pr2)) {
          add_error() << "Duplicate prefixes '" << pr1 << "' "
                      << "for " << p.name;
        }
      }
    }
  }

  // Long text must be present
  if (p.usage == nullptr || p.usage[0] == '\0') {
    add_error() << "Empty long text passed "
                << "in parameter '" << p.name << "'; this field is required";
  }
}


/**
 * @brief Check that the action-labels are unique.
 *
 * @return true if unique, false otherwise
 */
void CmdValidation::check_actions_distinct(DefActions &actions) {
  int length = (int) actions.size();

  for (int index1 = 0; index1 < length - 1; ++index1) {
    for (int index2 = index1 + 1; index2 < length; ++index2) {
      // Labels must be  unique
      if (actions[index1].name == actions[index2].name) {
        add_error() << "Multiple actions definitions with name '" << actions[index1].name << "'; "
                    << "labels should be unique";
      }
    }
  }
}
