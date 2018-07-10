#include "catch.hpp"
#include "../Lib/CmdParameter.h"
#include "Support/cout_redirect.h"
#include "TestData/TestParameters.h"

const char *PROG = "TestProg";  // Name of dummy executable


//////////////////////////////////////////////////
// Local helpers
//////////////////////////////////////////////////

/**
 * @brief Parse a command line with a single switch
 */
bool single_param(TestParameters &params, const char *in_switch) {
  int argc = 3;
  const char *argv[] = { PROG, in_switch, "input_file.txt"	};
  return params.handle_commandline(argc, argv, false);
}


//////////////////////////////////////////////////
// The actual unit tests
//////////////////////////////////////////////////



/**
 * TODO: check border cases for param's with limits
 */
TEST_CASE("Test Command Line parameters", "[params]") {
	cout_redirect redirect;
	TestParameters params;  // NOTE: putting this in global space causes a segfault


	SECTION("Check help") {
		int argc1 = 2;
		const char *argv1[] = { PROG, "-h"	};
		REQUIRE(params.handle_commandline(argc1, argv1, false));

		int argc2 = 9;
		const char *argv2[] = { PROG, "it", "doesn't", "matter",
														"-h", "what", "I", "put", "here"	};
		REQUIRE(params.handle_commandline(argc2, argv2, false));

		//redirect.clear();

		int argc3 = 9;
		const char *argv3[] = { PROG, "it", "doesn't", "matter",
														"what", "I", "put", "here", "-h"	};
		REQUIRE(params.handle_commandline(argc3, argv3, false));

		//redirect.close();
		//std::cout << "redirect: " << redirect.str() << std::endl;

		int argc4 = 11;
		const char *argv4[] = { PROG, "This", "-h", "should",
														"-h", "be", "-h", "no", "-h", "problem", "-h"	};
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
		REQUIRE(params.m_positive  != -1);
		REQUIRE(params.m_unsigned  != -1);
		REQUIRE(params.m_float     != -1.0f);

		// Test expected defaults
		REQUIRE(params.m_positive  == 1);
		REQUIRE(params.m_unsigned  == 0);
		REQUIRE(params.m_float     == 0.0f);  //Approx(3.1419).epsilon(0.00001));
		REQUIRE(params.input_file  == "input_file.txt");
		REQUIRE(params.output_file.empty());
		REQUIRE(params.m_bool == false);
		REQUIRE(params.bool_detected == false);
	}


	SECTION("Check set defaults of parameters") {
		// TODO
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

		REQUIRE(params.handle_commandline(argc1, argv1, false));

		REQUIRE(params.m_positive  == 123);
		REQUIRE(params.m_unsigned  == 42);
		REQUIRE(params.m_float     == Approx(3.1419).epsilon(0.00001));
		REQUIRE(params.input_file  == "input_file.txt");
		REQUIRE(params.output_file == "output_file.txt");
		REQUIRE(params.m_bool == true);
		REQUIRE(params.bool_detected == true);
	}


	SECTION("Check bad parameters") {
		// Each value here is tested separately.
		// They should all fail.
		const char *test_params[] = {
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
			"-output= ",
			nullptr
		};

		for (int n = 0; test_params[n] != nullptr; ++n) {
			INFO("Doing index " << n << ", value: " << test_params[n]);
			REQUIRE(!single_param(params, test_params[n]));
		}

		// Test border case(s)
		REQUIRE(single_param(params, "-positive=1"));
		REQUIRE(single_param(params, "-unsigned=0"));
		REQUIRE(single_param(params, "-output=123"));
	}


	SECTION("Same names for parameter definitions should not be allowed") {
		const char *usage = "blurb";

		DefParameter double_params[] = {
			{	"Name not unique", "", UNNAMED,	"" },
			{	"Name not unique", "", UNNAMED,	"" },
			nullptr
		};

		REQUIRE(!CmdParameter::init_params(usage, double_params));
	}

	SECTION("Indexed access of parameter values should return the same as keyed access") {
		CmdParameter::List &p = CmdParameter::parameters;

		const char *keys[] = {
			"Unsigned integer",
			"Positive integer",
			"Positive float",
			"Output file",
			"A boolean value",
			"An integer value",
			"Input file",
			nullptr
		};

		for (int n = 0; keys[n] != nullptr; ++n) {
			REQUIRE(p[n]->get_int_value() == p[keys[n]]->get_int_value());
		}
	}
}


