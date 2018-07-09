#include "StringParameter.h"
#include <cassert>
#include <string>
#include <sstream>


void StringParameter::default_indicator(std::ostringstream &os) override {
  os << def_param.string_default;
}
