#ifndef TESTPARAMETERS_H
#define TESTPARAMETERS_H
#include <string>
#include "../../Lib/CmdParameters.h"


class TestParameters {
  using string = std::string;

public:
  bool handle_commandline(
    int argc,
    const char* argv[],
    bool show_help_on_error = true);

	// These fields are intentionally initialized
	// to illegal values for unit testing.
	// The tested code should be able to deal with this.
	int    m_unsigned{-1};
	int	   m_positive{-1};
	float  m_float{-1};

	string input_file;
	string output_file;

	int    m_int;
	bool   m_bool;
	bool   bool_detected;

	CmdParameters::List &parameters();

private:
	void pass_params();
};

#endif // TESTPARAMETERS_H
