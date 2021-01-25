#include "OptionParameter.h"
#include "../DefParameter.h"
#include <cassert>
#include <cstring>  // strcmp()
#include <string>
#include <algorithm>
#include <sstream>
#include "../Support/debug.h"


namespace {

int indexOf(std::vector<char const *> const &vec, char const *value) {

  auto iter = std::find_if(vec.begin(), vec.end(), [&value] (char const *val) -> bool {
    return !strcmp(val, value);
  });

  size_t index = std::distance(vec.begin(), iter);
  if(index == vec.size()) {
    return -1; //invalid
  }

  return (int) index;
}

}  // anon namespace


OptionParameter::OptionParameter(DefParameter &var) :
  TypedParameter(var, "<opt>"),
  m_options(var.options()) {
  assert(m_options != nullptr);
  assert(!m_options->empty());

  m_defaults.int_value = 0;
  m_defaults.string_value = (*m_options)[0];
}


std::string OptionParameter::usage() {
  return Parent::usage() + param_usage_extra();
}


std::string OptionParameter::param_usage_extra() {
  std::string out = "\nAllowed values: ";
  bool did_first = false;
  int count = 0;
  for(auto const& v: *m_options) {
    if (did_first) {
      out += "  ";
    } else {
      did_first = true;
    }

    out += v;
    if (count == m_defaults.int_value) {
      out += "(default)";
    }

    count += 1;
  }

  out += "\n";

  return out;
}


bool OptionParameter::parse_param_internal(const std::string &in_value) {
  assert(m_options != nullptr);
  assert(!in_value.empty());
  assert(def_param.param_type == OPTION);

  int index = indexOf(*m_options, in_value.c_str());
  if (index == -1) {
    string msg = "Invalid value '";
    msg += in_value + "'";
    error(msg);
    return false;
  }


  // value is an option
  m_values.string_value = in_value;
  m_values.int_value = index;
  m_values.m_detected = true;

  return true;
}
