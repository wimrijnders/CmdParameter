#include "TestParameters.h"

namespace {

CmdParameters definition = {
  // Usage
  "TestProg test of unit testing command line parameters\n"
  "\n"
  "Usage:\n"
  "    TestProg input_file {options}\n"
  "\n"
  "Where:\n"
  "    input_file         - filename not passed in as parameter\n",

  // global parameters
  {{
    "Unsigned integer",
    "-unsigned=",
    UNSIGNED_INTEGER,
    "Some value which must not be negative"
  }, {
    "Positive integer",
    "-positive=",
    POSITIVE_INTEGER,
    "It can't be negative and also not zero."
  }, {
    "Positive float",
    "-float=",
    POSITIVE_FLOAT,
    "A float which can't be zero or smaller"
  }, {
    "Output file",
    "-output=",
    STRING,
    "The name of some file we want to output to"
  }, {
    "A boolean value",
    "-bool",
    NONE,
    "This value is just plain set. There is no value"
  }, {
    "An integer value",
    "-int=",
    INTEGER,
    "This value can be any integer"
  }, {
    "Input file",
    "first_file",
    UNNAMED,
    "The first unnamed parameter, which we take to be a filename"
  }}
};

}  // anon namespace


//////////////////////////////////////////////////
// Class TestParameters
//////////////////////////////////////////////////

/**
 * @return true if everything OK, false otherwise
 */
bool TestParameters::handle_commandline(
  int argc,
  const char* argv[],
  bool show_help_on_error) {

	auto ret = definition.handle_commandline(argc, argv, show_help_on_error);
	if (ret == CmdParameters::ALL_IS_WELL) {
		pass_params();
		return true;
	}

	return (ret != CmdParameters::EXIT_ERROR);
}


CmdParameters::List &TestParameters::parameters() { return definition.parameters(); }
std::string TestParameters::get_errors() const    { return  definition.get_errors(); }


void TestParameters::pass_params() {
  CmdParameters::List &p = parameters();

  // TODO: use keys here instead.
  m_unsigned  = p[0]->get_int_value();
  m_positive  = p[1]->get_int_value();
  m_float     = p[2]->get_float_value();
  output_file = p[3]->get_string_value();
  m_bool      = p[4]->get_bool_value();
  m_int       = p[5]->get_int_value();
  input_file  = p[6]->get_string_value();

  bool_detected = p[4]->detected();
}

