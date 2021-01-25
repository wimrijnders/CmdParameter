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

  // Intentionally initialized to illegal values for unit testing.
  // Should all be overwritten on initialization
  int    m_unsigned = -1;
  int    m_positive = -1;
  float  m_float    = -1;
  int    m_int      = -1;
  int    m_intdef   = -1;
  bool   m_bool     = true;

  string input_file;
  string output_file;


  CmdParameters::List &parameters();
  std::string get_errors() const;

private:
  void pass_params();
};

#endif // TESTPARAMETERS_H
