#include "catch.hpp"
#include "../Lib/CmdParameters.h"

TEST_CASE("Fixed options for param", "[options]") {
  SECTION("Options parameters should be accepted") {
    DefParameters a = {{
      "option",
      "-option=",
			{
				{ 1, "option1"},
				{ 2, "option2"},
				{ 3, "option3"}
			},
      "Option blurb."
    }};
    CmdParameters no_actions("blurb", a);
    REQUIRE(no_actions.init_params());

		// Test valid option
		int argc1 = 1;
		char const *argv1[] = {
			"-option=1"
		};

    REQUIRE(CmdParameters::ALL_IS_WELL == no_actions.handle_commandline(argc1, argv1));

		// Test invalid option
		int argc2 = 1;
		char const *argv2[] = {
			"-option=4"
		};

    REQUIRE(CmdParameters::ALL_IS_WELL != no_actions.handle_commandline(argc2, argv2));
  }
}
