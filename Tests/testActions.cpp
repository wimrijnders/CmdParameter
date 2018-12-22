#include "catch.hpp"
#include "TestData/TestActions.h"
#include "../Lib/CmdParameters.h"
#include "../Lib/TypedParameter.h"

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
    REQUIRE(!no_actions.init_params());
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
