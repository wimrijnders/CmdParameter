#include "StringParameter.h"
#include "../DefParameter.h"
#include <cassert>
#include <string>
#include <sstream>


void StringParameter::default_indicator(std::ostringstream &os) {
  os << def_param.string_default;
}
