#include "Types.h"
#include <cassert>
#include <iostream>
#include <sstream>   // ostringstream
#include "NoneParameter.h"
#include "IntParameter.h"
#ifndef LITE
#include "UnsignedIntParameter.h"
#include "PositiveIntParameter.h"
#include "PositiveFloatParameter.h"
#include "StringParameter.h"
#include "UnnamedParameter.h"
#endif  // LITE


CmdParameter *DefParameter_factory(DefParameter &item) {
  CmdParameter *p = nullptr;

  switch (item.param_type) {
  case NONE:             p = new NoneParameter(item);          break;
  case INTEGER:          p = new IntParameter(item);           break;
#ifndef LITE
  case UNSIGNED_INTEGER: p = new UnsignedIntParameter(item);   break;
  case POSITIVE_INTEGER: p = new PositiveIntParameter(item);   break;
  case POSITIVE_FLOAT:   p = new PositiveFloatParameter(item); break;
  case STRING:           p = new StringParameter(item);        break;
  case UNNAMED:          p = new UnnamedParameter(item);       break;
#endif  // LITE

  default: {
      std::ostringstream msg;
      msg << "Error: Unhandled parameter type " << item.param_type;
      std::cout << msg.str() << std::endl;
    }
  }

  assert(p != nullptr);
  return p;
}
