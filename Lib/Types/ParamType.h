#ifndef PARAMTYPE_H
#define PARAMTYPE_H

enum ParamType {
  NONE,             //> Parameter without a value, e.g. `-enabled`
  INTEGER,          //> Parameter takes any integer
  UNSIGNED_INTEGER, //> Parameter takes an integer >= 0
  POSITIVE_INTEGER, //> Parameter takes a integer > 0
  POSITIVE_FLOAT,   //> Parameter takes a float > 0
  STRING,           //> Parameter takes a string, e.g. `-output=filename.txt`
  OPTION,           //> Parameter takes a string from a list of options
  UNNAMED,          //> Not a switch parameter, but expected on the command line.
                    //  Order is important
  NUM_PARAMTYPES   // TODO assert valid type
};

#endif // PARAMTYPE_H
