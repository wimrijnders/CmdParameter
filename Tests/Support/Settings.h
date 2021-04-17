#ifndef _EXAMPLE_SUPPORT_SETTINGS_H
#define _EXAMPLE_SUPPORT_SETTINGS_H
#include <string>
#include <CmdParameters.h>

/**
 * Data structure used for the params sort unit test (**only**).
 *
 * Simplified version of the corresponding structure in V3DLib.
 */
struct Settings {
  Settings(CmdParameters *derived_params = nullptr, bool use_num_qpus = false);

  bool init(int argc, const char *argv[]);
  virtual bool init_params() { return true; }
  std::string get_usage() const;

private:
  CmdParameters * const m_derived_params;
  bool const m_use_num_qpus;

  bool process(CmdParameters &in_params);
  CmdParameters &base_params();
};

#endif  // _EXAMPLE_SUPPORT_SETTINGS_H
