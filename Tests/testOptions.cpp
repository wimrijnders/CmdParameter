#include "doctest.h"
#include "../Lib/CmdParameters.h"
#include "Support/debug.h"

namespace {

const char *PROG = "TestProg";  // Name of dummy executable

}  // anon namespace


TEST_CASE("Fixed options for param [options]") {
  SUBCASE("Options parameters should be accepted") {
    DefParameters a = {{
      "option",
      "-option=",
      {"opt1", "opt2", "opt3"},
      "Option blurb."
    }};
    CmdParameters no_actions("blurb", a);
    REQUIRE(no_actions.init());

    // Test missing option
    int argc1 = 1;
    char const *argv1[] = {
      PROG
    };

    //breakpoint
    REQUIRE(CmdParameters::ALL_IS_WELL == no_actions.handle_commandline(argc1, argv1, false));
    REQUIRE(no_actions.parameters()[0]->get_int_value() == 0);
    REQUIRE(no_actions.parameters()[0]->get_string_value() == "opt1");

    // Test valid option
    int argc2 = 2;
    char const *argv2[] = {
      PROG,
      "-option=opt2"
    };

    REQUIRE(CmdParameters::ALL_IS_WELL == no_actions.handle_commandline(argc2, argv2, false));
    REQUIRE(no_actions.parameters()[0]->get_int_value() == 1);
    REQUIRE(no_actions.parameters()[0]->get_string_value() == "opt2");

    // Test invalid option
    no_actions.silent(true);

    int argc3 = 2;
    char const *argv3[] = {
      PROG,
      "-option=opt4"
    };

    REQUIRE(CmdParameters::ALL_IS_WELL != no_actions.handle_commandline(argc3, argv3, false));

    no_actions.silent(false);
  }
}
