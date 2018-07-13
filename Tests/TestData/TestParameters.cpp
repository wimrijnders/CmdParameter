#include "TestParameters.h"
#include "../../Lib/CmdDefinition.h"
#include "../../Lib/CmdParameter.h"


CmdDefinition definition = {
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

	auto ret = CmdParameter::handle_commandline(definition, argc, argv, show_help_on_error);
	if (ret == CmdParameter::ALL_IS_WELL) {
		pass_params();
		return true;
	}

	return (ret != CmdParameter::EXIT_ERROR);
}


void TestParameters::pass_params() {
  CmdParameter::List &parameters = CmdParameter::parameters;

  // TODO: use keys here instead.
  m_unsigned  = parameters[0]->get_int_value();
  m_positive  = parameters[1]->get_int_value();
  m_float     = parameters[2]->get_float_value();
  output_file = parameters[3]->get_string_value();
  m_bool      = parameters[4]->get_bool_value();
  m_int       = parameters[5]->get_int_value();
  input_file  = parameters[6]->get_string_value();

  bool_detected = parameters[4]->detected();
}

