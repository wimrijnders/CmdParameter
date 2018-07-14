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
/*
  DefActions a;  // Somewhat contrived
  CmdDefinition no_actions("blurb", a);

  REQUIRE(!CmdParameter::init_params(no_actions));

  // TODO: check empty usage - following might not do that

  // The test actions should be okay
  REQUIRE(CmdParameter::init_params(defined_actions));
*/
  SECTION("No two actions named the same") {
    DefActions a = {{
      "action1",
      "This is action 1"
    }, {
      "action1",
      "This is action 2"
    }};
    CmdParameters no_actions("blurb", a);

    REQUIRE(!no_actions.init_params());
  }
}
