#include "catch.hpp"
#include "Support/debug.h"
#include "../Lib/CmdParameters.h"
#include "Support/cout_redirect.h"
#include "TestData/TestParameters.h"

const char *PROG = "TestProg";  // Name of dummy executable


//////////////////////////////////////////////////
// Local helpers
//////////////////////////////////////////////////

/**
 * @brief Parse a command line with a single switch
 */
void single_param(TestParameters &params, char const *in_switch, int index = -1, bool check_false = false) {
  int argc = 3;
  const char *argv[] = { PROG, in_switch, "input_file.txt"  };
  bool ret = params.handle_commandline(argc, argv, false);

  if (index != -1) {
    INFO("Doing index " << index << ", value: " << in_switch);
  }

  INFO(params.get_errors());

  if (check_false) {
    REQUIRE(!ret);
  } else {
    REQUIRE(ret);
  }
}


//////////////////////////////////////////////////
// The actual unit tests
//////////////////////////////////////////////////

TEST_CASE("Test parameter definitions", "[params]") {

  SECTION("null values not allowed") {
    // Reference value; this should succeed
    DefParameters a0 = {
      { "prefix1", "-switch2", NONE, "Long blurb."}
    };
    CmdParameters c0("blurb", a0);
    REQUIRE(c0.init());


    // Action 'nullptr' means that this is a global switch
    DefParameters a1 = {
      { nullptr, "-switch2", NONE, "Long blurb."}
    };
    CmdParameters c1("blurb", a1);
    REQUIRE(!c1.init());


    DefParameters a2 = {
      { "prefix1", nullptr, NONE, "Long blurb."}
    };
    CmdParameters c2("blurb", a2);
    REQUIRE(!c2.init());


    DefParameters a3 = {
      { "prefix1", "-a", NONE, nullptr}
    };
    CmdParameters c3("blurb", a3);
    REQUIRE(!c3.init());


    DefParameters a4 = {
      { "prefix2", "-b", NONE, ""}
    };
    CmdParameters c4("blurb", a4);
    REQUIRE(!c4.init());
  }


/*
  TODO

  SECTION("correct detection of assign postfix") {
    DefParameters a1 = {
      { "prefix1", "-switch=", NONE, "Long blurb."}
    };
    CmdParameters c1("blurb", a1);
    c1.show_errors(true);
    REQUIRE(!c1.init());
  }
*/
}


TEST_CASE("Test multiple prefixes", "[params]") {
  SECTION("Null values not allowed") {
    // Reference implementation - this should succeed
    DefParameters a0 = {
      { "prefix1", { "-switch1", "-switch2" }, NONE, "Long blurb."}
    };
    CmdParameters c0("blurb", a0);
    REQUIRE(c0.init());

    DefParameters a1 = {
      { "prefix1", { nullptr, "-switch2" }, NONE, "Long blurb."}
    };
    CmdParameters c1("blurb", a1);
    REQUIRE(!c1.init());

    DefParameters a2 = {
      { "prefix1", { "-switch1", nullptr }, NONE, "Long blurb."}
    };
    CmdParameters c2("blurb", a2);
    REQUIRE(!c2.init());

    // Action can not be null either
    DefParameters a3 = {
      { nullptr, { "-switch1", "-switch2" }, NONE, "Long blurb."}
    };
    CmdParameters c3("blurb", a3);
    REQUIRE(!c3.init());
  }

  SECTION("Prefixes can not be the same in prefix list") {
    DefParameters a1 = {
      { "prefix1", { "-switch1", "-switch2", "-switch1" }, NONE, "Long blurb."}
    };
    CmdParameters c1("blurb", a1);
    //c1.show_errors(true);
    REQUIRE(!c1.init());
  }

  SECTION("Prefixes can not be the same over prefixes in list") {
    // Reference, should succeed
    DefParameters a0 = {
      { "prefix1", { "-switch1", "-switch2", "-switch3" }, NONE, "Long blurb."},
      { "prefix2", { "-switch4", "-switch5", "-switch6" }, NONE, "Long blurb."},
    };
    CmdParameters c0("blurb", a0);
    CHECK(c0.init());

    DefParameters a1 = {
      { "prefix1", { "-switch1", "-switch2", "-switch3" }, NONE, "Long blurb."},
      { "prefix2", { "-switch4", "-switch2", "-switch6" }, NONE, "Long blurb."},
    };
    CmdParameters c1("blurb", a1);
    REQUIRE(!c1.init());
  }
}


TEST_CASE("Test chained parameter definitions", "[params]") {
  SECTION("Chained global parameters should work") {
    DefParameters a0 = {
      { "Parent definition", "-1", NONE, "This switch is in the parent."}
    };
    CmdParameters c0("Test chaining - parent", a0);
    c0.silent(true);

    DefParameters a1 = {
      { "Child definition", "-2", NONE, "This switch is in the child."}
    };
    CmdParameters c1("Test chaining - child", a1, &c0);
    INFO(c1.get_errors());
    REQUIRE(!c1.has_errors());

    //c1.show_usage();

    int argc1 = 2;
    const char *argv1[] = { PROG, "-1"};
    REQUIRE(c0.handle_commandline(argc1, argv1, false) == CmdParameters::ALL_IS_WELL);

    // Option -2 should not be available in parent
    int argc2 = 2;
    const char *argv2[] = { PROG, "-2"};
    REQUIRE(c0.handle_commandline(argc2, argv2, false) != CmdParameters::ALL_IS_WELL);

    // Both options should be available
    int argc3 = 3;
    const char *argv3[] = { PROG, "-1", "-2"};
    REQUIRE(c1.handle_commandline(argc3, argv3, false));
  }


  SECTION("Validation should pick up same params over chained definitions") {
    DefParameters a0 = {
      { "A definition", "-1", NONE, "This switch is in the parent."}
    };
    CmdParameters c0("Test chaining - parent", a0);
    REQUIRE(c0.init());

    // Single definition, should pass
    int argc1 = 2;
    const char *argv1[] = { PROG, "-1"};
    REQUIRE(c0.handle_commandline(argc1, argv1, false) == CmdParameters::ALL_IS_WELL);

    DefParameters a1 = {
      { "A definition", "-2", NONE, "This switch is in the child."}
    };
    CmdParameters c1("Test chaining - child", a1, &c0);
    c1.silent(true);
    // Should fail due to double definition
    REQUIRE(!c1.init());
    //printf("Chained double definition:\n%s\n", c1.get_errors().c_str());

    // Now a double definition - should fail
    // Both options should be available
    int argc3 = 2;
    const char *argv3[] = { PROG, "-1"};
    if (!c1.handle_commandline(argc3, argv3, false)) {
      INFO(c1.get_errors());
      REQUIRE(false);
    }
  }


  SECTION("Runtime adding of parameters should work") {
    int argc = 2;
    const char *argv1[] = { PROG, "-1"};
    const char *argv2[] = { PROG, "-2"};

    DefParameters a0 = {
      { "Option 1", "-1", NONE, "This switch is in the original."}
    };
    CmdParameters c0("Test add - initial", a0);

    DefParameters a1 = {
      { "Option 2", "-2", NONE, "This switch is in the added list."}
    };
    CmdParameters c1("Test add - added", a1);

    // Sanity checks
    REQUIRE(c0.handle_commandline(argc, argv1, false) == CmdParameters::ALL_IS_WELL);
    REQUIRE(c1.handle_commandline(argc, argv2, false) == CmdParameters::ALL_IS_WELL);

    // The thing we want to test 
    //c0.show_usage();
    REQUIRE(c0.handle_commandline(argc, argv2, false) != CmdParameters::ALL_IS_WELL);  // -2 not present yet

    if (!c0.add(c1)) {
      INFO(c0.get_errors());
      REQUIRE(false);
    }

    //c0.show_usage();
    REQUIRE(c0.handle_commandline(argc, argv2, false) == CmdParameters::ALL_IS_WELL);  // -2 now present

    // Doubles in added parameters should not be accepted
    REQUIRE(!c0.add(c1));  // We already added this
  }
}


TEST_CASE("Test Command Line parameters", "[params]") {
  cout_redirect redirect;
  TestParameters params;  // NOTE: putting this in global space causes a segfault

  SECTION("Check help short switch") {
    int argc1 = 2;
    const char *argv1[] = { PROG, "-h"};
    REQUIRE(params.handle_commandline(argc1, argv1, false));

    int argc2 = 9;
    const char *argv2[] = { PROG, "it", "doesn't", "matter", "-h", "what", "I", "put", "here"};
    REQUIRE(params.handle_commandline(argc2, argv2, false));

    int argc3 = 9;
    const char *argv3[] = { PROG, "it", "doesn't", "matter", "what", "I", "put", "here", "-h"};
    REQUIRE(params.handle_commandline(argc3, argv3, false));

    int argc4 = 11;
    const char *argv4[] = { PROG, "This", "-h", "should", "-h", "be", "-h", "no", "-h", "problem", "-h"};
    REQUIRE(params.handle_commandline(argc4, argv4, false));
  }


  SECTION("Check help long switch") {
    int argc1 = 2;
    const char *argv1[] = { PROG, "help"  };
    REQUIRE(params.handle_commandline(argc1, argv1, false));

    int argc2 = 9;
    const char *argv2[] = { PROG, "it", "doesn't", "matter",
                            "help", "what", "I", "put", "here"  };
    REQUIRE(params.handle_commandline(argc2, argv2, false));

    int argc3 = 9;
    const char *argv3[] = { PROG, "it", "doesn't", "matter",
                            "what", "I", "put", "here", "help"  };
    REQUIRE(params.handle_commandline(argc3, argv3, false));

    int argc4 = 11;
    const char *argv4[] = { PROG, "This", "help", "should",
                            "help", "be", "help", "no", "help", "problem", "help"  };
    REQUIRE(params.handle_commandline(argc4, argv4, false));
  }


  SECTION("Check help both long and short switches") {
    int argc4 = 11;
    const char *argv4[] = { PROG, "This", "-h", "should",
                            "help", "be", "-h", "no", "-h", "problem", "help"  };
    REQUIRE(params.handle_commandline(argc4, argv4, false));
  }


  SECTION("Check unset defaults of parameters") {
    int argc1 = 2;
    const char *argv1[] = {
      PROG,
      "input_file.txt",
    };

    REQUIRE(params.handle_commandline(argc1, argv1, false));

    // Test illegal cases
    REQUIRE(params.m_unsigned  != -1);
    REQUIRE(params.m_positive  != -1);
    REQUIRE(params.m_float     != -1.0f);
    REQUIRE(params.m_int       != -1);
    REQUIRE(params.m_intdef    != -1);

    // Test expected defaults
    REQUIRE(params.m_unsigned  == 0);
    REQUIRE(params.m_positive  == 1);
    REQUIRE(params.m_float     == 1.0f);  //Approx(3.1419).epsilon(0.00001));
    REQUIRE(params.m_int       == 0);
    REQUIRE(params.m_intdef    == 42);
    REQUIRE(params.m_bool      == false);
    REQUIRE(params.input_file  == "input_file.txt");
    REQUIRE(params.output_file.empty());
  }


  SECTION("Check good parameters") {
    int argc1 = 8;
    const char *argv1[] = {
      PROG,
      "-int=-246",
      "-positive=123",
      "-unsigned=42",
      "-float=3.1419",
      "input_file.txt",
      "-output=output_file.txt",
      "-bool"
    };

    if (!params.handle_commandline(argc1, argv1, false)) {
      INFO(params.get_errors());
      REQUIRE(false);
    }

    REQUIRE(params.m_positive  == 123);
    REQUIRE(params.m_unsigned  == 42);
    REQUIRE(params.m_float     == Approx(3.1419).epsilon(0.00001));
    REQUIRE(params.input_file  == "input_file.txt");
    REQUIRE(params.output_file == "output_file.txt");
    REQUIRE(params.m_bool == true);
  }


  SECTION("Check bad parameters") {
    //redirect.clear();

    // Each value here is tested separately.
    // They should all fail.
    const char *test_params[] = {
      "-int",        // Value required
      "-int=",

      "-bool=1",     // Take no value
      "-bool=",
      "-bool=false",
      "-h=",
      "-h=1",

      "-int=\t123",
      "-int=meow",
      "-int=3.1419",
      "-i_dont_exist",
      "-i_dont_exist=hello",
      "-positive=meow",
      "-unsigned= 1",
      "-positive=-1",
      "-positive=0",
      "-positive=1.234",
      "-unsigned=-1",
      "unsigned=1",
      "-float=meow",
      "-float=-0.01",
      "-float=3.14string",
      "-output=",
      "-output= ",
      nullptr
    };

    for (int n = 0; test_params[n] != nullptr; ++n) {
      single_param(params, test_params[n], n, true);
    }

    //redirect.close();
    //std::cout << "redirect: " << redirect.str() << std::endl;
  }


  SECTION("Check border cases") {
    single_param(params, "-positive=1");
    single_param(params, "-unsigned=0");
    single_param(params, "-output=123");
  }


  SECTION("Same names for parameter definitions should not be allowed") {
    CmdParameters double_params = {
      "blurb", {  // Usage
      {  "Name not unique", "-a", UNNAMED,  "" },
      {  "Name not unique", "-b", UNNAMED,  "" }
    }};

    REQUIRE(!double_params.init());
  }


  SECTION("Empty name should not be allowed") {
    CmdParameters c = {
      "blurb", {  // Usage
      {  "", "-a", UNNAMED,  "" },
    }};

    REQUIRE(!c.init());
  }


  SECTION("Empty prefixes should not be allowed") {
    CmdParameters c = {
      "blurb", {  // Usage
      {  "Name", "", UNNAMED,  "" },
    }};

    REQUIRE(!c.init());
  }

  SECTION("Indexed access of parameter values should return the same as keyed access") {
    CmdParameters::List &p = params.parameters();

    const char *keys[] = {
      "Unsigned integer",
      "Positive integer",
      "Positive float",
      "Output file",
      "A boolean value",
      "An integer value",
      "An integer value with a default",
      "Input file",
      nullptr
    };

    for (int n = 0; keys[n] != nullptr; ++n) {
      INFO("Index: " << n << " - " << p[n]->get_int_value() << " == " << p[keys[n]]->get_int_value());
      REQUIRE(p[n]->get_int_value() == p[keys[n]]->get_int_value());
    }
  }
}


TEST_CASE("Test issues during usage", "[params][issues]") {

  CmdParameters base_params = {
    "blurb",
    {{
      "Disable logging",
      { "-s", "-silent" },
      ParamType::NONE,
      "Do not show the logging output on standard output"
    }}
  };

  SECTION("Check error 'Duplicate prefixes'") {
    CmdParameters p(base_params);
    INFO(p.get_errors());
    REQUIRE(!p.has_errors());
  }
}


