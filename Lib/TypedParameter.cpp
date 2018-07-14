/**
 * Classes for the handling of command line parameters.
#ifndef LITE
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
#endif  // LITE
 */
#include "TypedParameter.h"
#include <cassert>
#include <iostream>
#include <vector>
#include <cstring>   // strcmp
#include "Support/Strings.h"
#include "DefParameter.h"
#include "CmdParameters.h"

using namespace std;


//////////////////////////////////////////////
// Class CmdParameter::List
//////////////////////////////////////////////



/**
 * Grumbl need to redefine this after adding the key version.
 */
TypedParameter *TypedParameter::List::operator[] (int index) {
  return at(index).get();
}


TypedParameter *TypedParameter::List::operator[] (const char *key) {
  assert(key != nullptr);

  for (auto &ptr : *this) {
    auto &field = *ptr.get();
    if (0 == strcmp(field.def_param.name, key)) {
      return &field;
    }
  }

  string msg = "'";
  throw msg + key + "' is not a short name of a parameter.";
  return nullptr;
}


/**
 * @brief Prepare the switch and value part of the usage line,
 *        so that we can determine their max width for formatting.
 *
 * Also create display of defaults.
 */
void TypedParameter::List::prepare_usage(
  vector<string> &disp_defaults,
  vector<string> &disp_params,
  bool add_help) {

  auto add_param = [&] (
    TypedParameter &param,
    const string &value_indicator,
    std::ostringstream &default_indicator) {
    string tmp;
    if (param.def_param.prefix != nullptr) {
      tmp = param.def_param.prefix;
    }
    tmp += value_indicator;
    disp_params.push_back(tmp);

    if (default_indicator.str().empty()) {
      disp_defaults.push_back(".");
    } else {
      std::ostringstream buf;
      buf << "; default '" << default_indicator.str() << "'.";
      disp_defaults.push_back(buf.str());
    }
  };


  if (add_help) {
    // Internal help switch definition
    string value_indicator;
    std::ostringstream default_indicator;

    add_param(CmdParameters::help_switch, value_indicator, default_indicator);
  }


  for (auto &item : *this) {
    TypedParameter &param = *item;

    string value_indicator;
    std::ostringstream default_indicator;

    value_indicator = param.value_indicator();
    param.default_indicator(default_indicator);

    add_param(param, value_indicator, default_indicator);
  }
}


#ifndef LITE
bool TypedParameter::List::process_unnamed(const char *curarg) {
  for (auto &item: *this) {
    TypedParameter &param = *item;
    if (param.def_param.param_type != UNNAMED) continue;
    if (param.string_value.empty()) {
      param.string_value = curarg;
      return true;
    }
  }

  return false;
}


#endif  // LITE
//////////////////////////////////////////////
// Class CmdParameter
//////////////////////////////////////////////

TypedParameter::TypedParameter(DefParameter &var) :
  def_param(var),
  m_detected(false) {

	// Remove '=' from the field prefix
	auto tmp = Strings::explode(var.prefix, '=');
	if (!tmp.empty()) {
		m_prefix = tmp[0];
	}
	assert(!m_prefix.empty());

  set_default();
}


void TypedParameter::error(const string &msg) const {
	string pre("Parameter '");
	pre += def_param.name;
	pre += "' (" + m_prefix + ") ";
	throw string(pre + msg);
}


bool TypedParameter::parse_param(const char *curarg) {
  if (!Strings::starts_with(curarg, m_prefix)) {
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
        throw string(msg + " no value present and default not specified.");
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
    string msg("The value for field '");
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


bool TypedParameter::parse_bool_param(const string &in_value) {
	assert(in_value.empty());
	assert(def_param.param_type == NONE);

#ifndef LITE
  bool_value = true;
#endif  // LITE
  m_detected = true;
  return true;
}
#ifndef LITE


bool TypedParameter::parse_string_param(const string &in_value) {
	assert(!in_value.empty());
	assert(def_param.param_type == STRING);

  string_value = in_value;
  m_detected = true;
  return true;
}


float TypedParameter::get_float_value(const string &param) {
  float value = -1;
  const char *str = param.c_str();
  char *end = nullptr;
  value = (float) strtod(str, &end);

  if (end == str || *end != '\0') {
    string msg(def_param.name);
    throw string(msg + " value not a float.");
  }

  return value;
}


#endif  // LITE
bool TypedParameter::set_default() {
  if (def_param.is_int_type()) {
    if (def_param.int_default != DefParameter::INT_NOT_SET) {
      // Use default instead
      int_value = def_param.int_default;
      return true;
    }
#ifndef LITE
  } else if (def_param.is_float_type()) {
    if (def_param.float_default != DefParameter::FLOAT_NOT_SET) {
      // Use default instead
      float_value = def_param.float_default;
      return true;
    }
#endif  // LITE
  } else {
    // All other cases for now: not handled
    // TODO: see if explicit default settings is needed for these types
    return false;
  }

  return false;
}

