/**
 * Classes for the handling of command line parameters.
 *
 * ------------------------------------------------------------
 * ## TODO
 *
 * - Consider adding required parameters (both switches and unnamed)
 *   I.e. *must* be specified, not use default.
 * - Perhaps sort switches alphabetically
 *
 * ------------------------------------------------------------
 * ## TESTS
 *
 * - unnamed params: required or not/arbitrary number or limit
 */
#include "TypedParameter.h"
#include <cassert>
#include <cstring>   // strcmp
#include <iostream>
#include "Support/Exception.h"
#include "Support/basics.h"
#include "DefParameter.h"
#include "CmdParameters.h"
#include "lib_local.h"

using namespace std;


//////////////////////////////////////////////
// Class TypedParameter::List
//////////////////////////////////////////////

/**
 * Grumbl need to redefine this after adding the key version.
 */
TypedParameter *TypedParameter::List::operator[] (int index) {
  return at(index).get();
}

// grumbl
TypedParameter const *TypedParameter::List::operator[] (int index) const {
  return at(index).get();
}


TypedParameter const *TypedParameter::List::operator[] (const char *key) const {
  assert(key != nullptr);

  for (auto &ptr : *this) {
    auto &field = *ptr.get();
    if (0 == strcmp(field.def_param.name, key)) {
      return &field;
    }
  }

  string msg = "'";
  throw Exception(msg + key + "' is not a short name of a parameter.");
  return nullptr;
}


bool TypedParameter::List::process_unnamed(const char *curarg) {
  for (auto &item: *this) {
    TypedParameter &param = *item;
    if (param.def_param.param_type != UNNAMED) continue;

    if (param.m_values.string_value.empty()) {
      param.m_values.string_value = curarg;
      return true;
    } else if (param.m_values.string_value == curarg) {
      printf("Parameter already there! Not expecting this: %s\n", curarg);
      assert(false);  // Not expecting this any more

      // NOTE: Already filled in. Apparently, this can be called twice.
      //       As long as there are no side-effects, I don't mind.
      return true;
    }
  }

  return false;
}


void TypedParameter::List::reset_values() {
  for (auto &item: *this) {
    TypedParameter &param = *item;
    param.reset_values();
  }
}


//////////////////////////////////////////////
// Class TypedParameter::SortedList
//////////////////////////////////////////////

TypedParameter::SortedList::SortedList(TypedParameter::List const &parameters) {
  resize(parameters.size());

  int index = 0;
  for (auto &item: parameters) {
    (*this)[index] = item.get();
    index++;
  }

  std::sort(begin() , end(), [](TypedParameter const *a, TypedParameter const *b) {
    return *a < *b; }
  );
}


StrList TypedParameter::SortedList::prefixes_str(bool add_help) const {
  StrList ret;

  if (add_help) {
    // Internal help switch definition, always on top
    TypedParameter const &param = CmdParameters::help_switch;
    ret.push_back(param.prefixes_str());
  }

  for (auto &item : *this) {
    TypedParameter const &param = *item;
    ret.push_back(param.prefixes_str());
  }

  return ret;
}


StrList TypedParameter::SortedList::defaults_str(bool add_help) const {
  StrList ret;

  if (add_help) {
    // Internal help switch definition, always on top
    TypedParameter const &param = CmdParameters::help_switch;
    ret.push_back(param.defaults_str());
  }

  for (auto &item : *this) {
    TypedParameter const &param = *item;
    ret.push_back(param.defaults_str());
  }

  return ret;
}


//////////////////////////////////////////////
// Class TypedParameter
//////////////////////////////////////////////

TypedParameter::TypedParameter(DefParameter &var, char const *value_indicator) :
  def_param(var),
  m_value_indicator(value_indicator) {

  assert(value_indicator != nullptr);

  // Remove '=' from the field prefix
  // NOTE: var.prefixes may be empty! (incorrect but possible, checks should catch this)
  for (auto &p : var.prefixes) {
    auto tmp = Strings::explode(p, '=');
    if (!tmp.empty()) {
      m_prefixes.push_back(tmp[0]);
    }
  }
}


void TypedParameter::reset_values() {
  m_values = m_defaults;
}


void TypedParameter::error(const string &msg) const {
  string pre("Parameter '");
  pre += def_param.name;
  pre += "' (" + m_prefixes[0] + ") ";  // NOTE: uses just first prefix definition
  throw Exception(pre + msg);
}


bool TypedParameter::parse_param(const char *curarg) {
  bool found_prefix = false;

  for (auto &p : m_prefixes) {
    if (Strings::starts_with(curarg, p)) {
      if (p == curarg) {
        // exact match, all is well
        found_prefix = true;
        break;
      }

      if (takes_value()) {
        // The only character allowed after is an equals sign
        assert(strlen(curarg) > p.size());
        if (curarg[p.size()] != '=') continue;
      }

      found_prefix = true;
      break;
    }
  }

  if (!found_prefix) {
    return false;
  }

  string value = get_param(curarg);

  if (takes_value()) {
    if (value.empty()) {
      error("takes a value, none specified.");
    } else {
      // Disallow whitespace after '='
      if (std::isspace(static_cast<unsigned char>(value[0]))) {
        error(" has unexpected whitespace after '='.");
      }
    }
  } else {
    if (Strings::contains(curarg, "=") || !value.empty()) {
     error("has value specified, shouldn't have one.");
    }
  }

/*
  // All param's except type NONE should have a value specified
    if (value.empty()) {
      if (def_param.has_default()) {
        return true;  // All is well, we have a default
      } else {
        throw Exception(msg + " no value present and default not specified.");
      }
    }
*/

  return parse_param_internal(value);
}


int TypedParameter::get_int_value(const string &param) {
  int value = -1;

  const char *str = param.c_str();
  char *end = nullptr;
  value = (int) strtol(str, &end, 10);

  if (end == str || *end != '\0') {
    error("is not a number.");
  }

  return value;
}


string TypedParameter::get_param(const char *curarg) {
  string ret;

  string::size_type loc = string(curarg).find("=");
  if (loc != string::npos) {
    ret = string(curarg).substr(loc + 1);
  }

  return ret;
}


bool TypedParameter::parse_string_param(const string &in_value) {
  assert(!in_value.empty());
  assert(def_param.param_type == STRING);

  m_values.string_value = in_value;
  m_values.m_detected = true;
  return true;
}


float TypedParameter::get_float_value(const string &param) {
  float value = -1;
  const char *str = param.c_str();
  char *end = nullptr;
  value = (float) strtod(str, &end);

  if (end == str || *end != '\0') {
    string msg(def_param.name);
    throw Exception(msg + " value not a float.");
  }

  return value;
}


string TypedParameter::usage() const { return def_param.usage; }


bool TypedParameter::operator<(TypedParameter const &rhs) const {
  assert(!m_prefixes.empty());
  assert(!rhs.m_prefixes.empty());

  return (m_prefixes[0] < rhs.m_prefixes[0]);
}


std::string TypedParameter::prefixes_str() const {
  std::string ret;

  for (auto &p : def_param.prefixes) {
    assert(p != nullptr);

    if (!ret.empty()) {
      ret << ", ";
    }

    ret << p << m_value_indicator;
  }

  return ret;
}


std::string TypedParameter::defaults_str() const {
  std::string ret;

    if (default_indicator().empty()) {
      ret << ".";
    } else {
      ret << "; default '" << default_indicator() << "'.";
    }

  return ret;
}
