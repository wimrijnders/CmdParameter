#include "OptionParameter.h"
#include "../DefParameter.h"
#include <cassert>
#include <cstring>	// strcmp()
#include <string>
#include <algorithm>
#include <sstream>
//#include <iostream>


namespace {

int indexOf(std::vector<char const *> const &vec, char const *value) {
	//std::cout << "indexOf() value: " << value << std::endl;
	//std::cout << "indexOf() vec size: " << vec.size() << std::endl;

	auto iter = std::find_if(vec.begin(), vec.end(), [&value] (char const *val) -> bool {
		//std::cout << "indexOf() val: " << val << std::endl;
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
	TypedParameter(var),
	m_options(var.options()) {
	assert(m_options != nullptr);
	assert(!m_options->empty());

	//std::cout << "entered OptionParameter ctor" << std::endl;
	//std::cout << "ctor() options size: " << m_options->size() << std::endl;

	int_value = 0;   // TODO: This is temporary, allow an assigned default value
}


void OptionParameter::default_indicator(std::ostringstream &os) {
	// Do nothing; don't show default here
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
		if (count == int_value) {
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

	//std::cout << "entered parse_param_internal(), value: " << in_value << std::endl;

	int index = indexOf(*m_options, in_value.c_str());
	//std::cout << "index: " << index << std::endl;
	if (index == -1) {
		string msg = "Invalid value '";
		msg += in_value + "'";
		error(msg);
		return false;
	}


	// value is an option
  string_value = in_value;
  int_value = index;
  m_detected = true;

  return true;
}


bool OptionParameter::set_default() {
	assert(def_param.int_default != DefParameter::INT_NOT_SET);
	assert(def_param.int_default == 0);  // Current setting, warn me if this changes

	int_value = def_param.int_default;
	return true;
}
