//
// Basic command line handling for the example programs.
//
// This contains common functionality for all examples.
//
///////////////////////////////////////////////////////////////////////////////
#include "Settings.h"
#include <cassert>
#include <memory>
#include <iostream>

namespace {

// ============================================================================
// Settings 
// ============================================================================

const char *blurb = "Example Program\n" ;


CmdParameters base_params = {
  blurb,
  {{
    "Output Generated Code",
    "-f",
    ParamType::NONE,     // Prefix needed to disambiguate
    "Write representations of the generated code to file"
  }, { 
    "Compile Only",
    "-c",
    ParamType::NONE,
    "Compile the kernel but do not run it"
  }, {
    "Select run type",
    "-r=",
    {"default", "emulator", "interpreter"},
    "Run the kernel on the QPU, emulator or on the interpreter"
  }, {
    "Disable logging",
    {"-s", "-silent"},
    ParamType::NONE,
    "Do not show the logging output on standard output"
#ifdef QPU_MODE
    }, {
    "Performance Counters",
    "-pc",
    ParamType::NONE,
    "Show the values of the performance counters"
#endif  // QPU_MODE
    }, {
    "QPU timeout",
    { "-t=", "-timeout="},
    ParamType::POSITIVE_INTEGER,
    "Time in seconds to wait for a result to come back from the QPUs",
    10
  }}
};


CmdParameters numqpu_params = {
  "",
  {{
    "Num QPU's",
    "-n=",
    INTEGER,
    "Number of QPU's to use. The values depends on the platform being run on:\n"
    "  - vc4 (Pi3+ and earlier), emulator: an integer value from 1 to 12 (inclusive)\n"
    "  - v3d (Pi4)                       : 1 or 8\n",
    1
  }}
};


std::unique_ptr<CmdParameters> params;

CmdParameters &instance(bool use_numqpus = false) {
  if (!params) {
    CmdParameters *p = new CmdParameters(base_params);

    if (use_numqpus) {
      p->add(numqpu_params);
    }

    params.reset(p);
  }

  return *params;
}

}  // anon namespace


Settings::Settings(CmdParameters *derived_params, bool use_num_qpus) :
  m_derived_params(derived_params),
  m_use_num_qpus(use_num_qpus)
{}


CmdParameters &Settings::base_params() {
  return instance(m_use_num_qpus);
}


/**
 * Parse the params from the commandline.
 */
bool Settings::init(int argc, const char *argv[]) {
  int ret = CmdParameters::ALL_IS_WELL;

  CmdParameters *params = nullptr;
  if (m_derived_params != nullptr) {
    m_derived_params->add(base_params());
    params = m_derived_params;
  } else {
    params = &instance();
  }

  if (params->has_errors()) {
    std::cout << params->get_errors();
    ret = CmdParameters::EXIT_ERROR;
  } else {
    ret = params->handle_commandline(argc, argv, false);

    if (ret == CmdParameters::ALL_IS_WELL) {
      bool success = process(*params) && init_params();
      if (!success) {
        ret = CmdParameters::EXIT_ERROR;
      }
    }
  }

  return (ret != CmdParameters::EXIT_ERROR);
}


bool Settings::process(CmdParameters &in_params) {
  return true;  // Doing nothing here for now
}


std::string Settings::get_usage() const {
  assert(m_derived_params != nullptr);
  return m_derived_params->get_usage();
}
