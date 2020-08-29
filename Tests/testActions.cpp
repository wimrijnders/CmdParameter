#include "catch.hpp"
#include "Support/debug.h"
#include "TestData/TestActions.h"
#include "../Lib/CmdParameters.h"
#include "../Lib/TypedParameter.h"


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
    CmdParameters params("blurb", a);
    REQUIRE(!params.init());
  }
}


TEST_CASE("Test good action command lines", "[actions]") {
  int argc1 = 3;
  const char *argv1[] = { "Test", "action1", "-unsigned=51" };
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
		child_actions.silent(true);
    INFO(child_actions.get_errors());
    REQUIRE(!child_actions.has_errors());
		//child_actions.show_usage();

		// Both options should be available
		int argc1 = 2;
		const char *argv1[] = { "PROG", "child_action"};
		REQUIRE(child_actions.handle_commandline(argc1, argv1, false) == CmdParameters::ALL_IS_WELL);

		int argc2 = 2;
		const char *argv2[] = { "PROG", "parent_action"};
		REQUIRE(child_actions.handle_commandline(argc2, argv2, false) == CmdParameters::ALL_IS_WELL);

		int argc3 = 2;
		const char *argv3[] = { "PROG", "unknown_action"};
		REQUIRE(child_actions.handle_commandline(argc3, argv3, false) != CmdParameters::ALL_IS_WELL);
	}


	SECTION("Validation should pick up same actions over chained definitions") {
    DefActions a = {{
      "some_action",
      "This is the action in the parent",
      "Long blurb."
    }};
    CmdParameters parent_actions("blurb", a);
    REQUIRE(parent_actions.init());  // Single definition, should pass

    DefActions b = {{
      "some_action",
      "This is the action in the child",
      "Long blurb."
    }};

    CmdParameters child_actions("blurb", b, &parent_actions);
    REQUIRE(!child_actions.init());  // Should fail due to double definition
	}
}


TEST_CASE("Test bad action command lines", "[actions]") {
	defined_actions.silent(true);

  // An action *must* be present
  int argc1 = 1;
  const char *argv1[] = { "Test" };
  REQUIRE(CmdParameters::ALL_IS_WELL != defined_actions.handle_commandline(argc1, argv1, false));

  // No multiple actions
  int argc2 = 3;
  const char *argv2[] = { "Test", "action1", "action2" };
  REQUIRE(CmdParameters::ALL_IS_WELL != defined_actions.handle_commandline(argc2, argv2, false));

	defined_actions.silent(false);
}
