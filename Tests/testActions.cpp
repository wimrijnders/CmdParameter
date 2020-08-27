#include "catch.hpp"
#include <signal.h>  // raise(SIGTRAP)
#include "TestData/TestActions.h"
#include "../Lib/CmdParameters.h"
#include "../Lib/TypedParameter.h"

#define breakpoint raise(SIGTRAP);


//
// TODO: something similar for parameters
//
// TODO: should no actions + global params be OK?
//
TEST_CASE("Test bad action definitions", "[actions]") {
  SECTION("No two actions named the same") {
    DefActions a = {{
      "action1",
      "This is action 1",
      "Long blurb."
    }, {
      "action1",
      "This is also action 1",
      "Long blurb."
    }};
    CmdParameters no_actions("blurb", a);
    REQUIRE(!no_actions.init());
  }
}


TEST_CASE("Test good action command lines", "[actions]") {
  int argc1 = 3;
  const char *argv1[] = { "Test", "action1", "-unsigned=51" };
	//breakpoint;
  REQUIRE(CmdParameters::ALL_IS_WELL == defined_actions.handle_commandline(argc1, argv1, false));

  int argc2 = 2;
  const char *argv2[] = { "Test", "action2" };
  REQUIRE(CmdParameters::ALL_IS_WELL == defined_actions.handle_commandline(argc2, argv2, false));

  int argc3 = 4;
  const char *argv3[] = { "Test", "action3", "-unsigned=42", "-str=foo" };
  REQUIRE(CmdParameters::ALL_IS_WELL == defined_actions.handle_commandline(argc3, argv3, false));
}


TEST_CASE("Test chained action definitions", "[actions]") {
	SECTION("Chained actions should work") {
    DefActions a = {{
      "parent_action",
      "This is the action in the parent",
      "Long blurb."
    }};

    CmdParameters parent_actions("blurb", a);

    DefActions b = {{
      "child_action",
      "This is the action in the child",
      "Long blurb."
    }};

    CmdParameters child_actions("blurb", b, &parent_actions);
    REQUIRE(child_actions.init());

		child_actions.show_usage();

		// Both options should be available
		int argc1 = 2;
		const char *argv1[] = { "PROG", "child_action"};
		REQUIRE(child_actions.handle_commandline(argc1, argv1, false) == CmdParameters::ALL_IS_WELL);

		int argc2 = 2;
		const char *argv2[] = { "PROG", "parent_action"};
//breakpoint
		REQUIRE(child_actions.handle_commandline(argc2, argv2, false) == CmdParameters::ALL_IS_WELL);

		int argc3 = 2;
		const char *argv3[] = { "PROG", "unknown_action"};
		REQUIRE(child_actions.handle_commandline(argc3, argv3, false) != CmdParameters::ALL_IS_WELL);
	}
}


TEST_CASE("Test bad action command lines", "[actions]") {
  // An action *must* be present
  int argc1 = 1;
  const char *argv1[] = { "Test" };
  REQUIRE(CmdParameters::ALL_IS_WELL != defined_actions.handle_commandline(argc1, argv1, false));

  // No multiple actions
  int argc2 = 3;
  const char *argv2[] = { "Test", "action1", "action2" };
  REQUIRE(CmdParameters::ALL_IS_WELL != defined_actions.handle_commandline(argc2, argv2, false));
}
