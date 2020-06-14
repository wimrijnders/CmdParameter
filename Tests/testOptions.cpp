#include "catch.hpp"
#include "../Lib/CmdParameters.h"

namespace {
const char *PROG = "TestProg";  // Name of dummy executable
}  // anon namespace


TEST_CASE("Fixed options for param", "[options]") {
  SECTION("Options parameters should be accepted") {
    DefParameters a = {{
      "option",
      "-option=",
			{"opt1", "opt2", "opt3"},
      "Option blurb."
    }};
    CmdParameters no_actions("blurb", a);
    REQUIRE(no_actions.init_params());

		// Test valid option
		int argc1 = 2;
		char const *argv1[] = {
			PROG,
			"-option=opt2"
		};

    REQUIRE(CmdParameters::ALL_IS_WELL == no_actions.handle_commandline(argc1, argv1, false));
		REQUIRE(no_actions.parameters()[0]->get_int_value() == 1);
		REQUIRE(no_actions.parameters()[0]->get_string_value() == "opt2");

		// Test invalid option
		int argc2 = 2;
		char const *argv2[] = {
			PROG,
			"-option=opt4"
		};

    REQUIRE(CmdParameters::ALL_IS_WELL != no_actions.handle_commandline(argc2, argv2, false));
  }
}
