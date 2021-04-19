#include "Types.h"
#include <cassert>
#include <iostream>
#include "../DefParameter.h"
#include "NoneParameter.h"
#include "IntParameter.h"
#include "OptionParameter.h"
#include "UnsignedIntParameter.h"
#include "PositiveIntParameter.h"
#include "PositiveFloatParameter.h"
#include "StringParameter.h"
#include "UnnamedParameter.h"
#include "Support/basics.h"


TypedParameter *DefParameter_factory(DefParameter &item) {
  TypedParameter *p = nullptr;

  switch (item.param_type) {
  case NONE:             p = new NoneParameter(item);          break;
  case INTEGER:          p = new IntParameter(item);           break;
  case OPTION:           p = new OptionParameter(item);        break;
  case UNSIGNED_INTEGER: p = new UnsignedIntParameter(item);   break;
  case POSITIVE_INTEGER: p = new PositiveIntParameter(item);   break;
  case POSITIVE_FLOAT:   p = new PositiveFloatParameter(item); break;
  case STRING:           p = new StringParameter(item);        break;
  case UNNAMED:          p = new UnnamedParameter(item);       break;

  default: {
      std::string msg;
      msg << "Error: Unhandled parameter type " << item.param_type;
      std::cout << msg << std::endl;
    }
  }

  assert(p != nullptr);
  return p;
}
