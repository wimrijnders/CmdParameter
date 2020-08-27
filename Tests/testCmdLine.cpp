#include "catch.hpp"
#include <cstdlib>
#include "Support/cout_redirect.h"


//
// Get the base directory right for calling stuff on the command line
//
#ifdef DEBUG
	#define POSTFIX_DEBUG "-debug"
#else
	#define POSTFIX_DEBUG ""
#endif

#define BIN_PATH "obj" POSTFIX_DEBUG "/bin"


TEST_CASE("Check correct working Simple app", "[cmdline]") {
	#define APP BIN_PATH "/Simple "
	#define POST " > /dev/null"

	REQUIRE(0 == system(APP POST));
	REQUIRE(0 == system(APP "-h" POST));
	REQUIRE(0 == system(APP "useless -h garbage" POST));
	REQUIRE(0 == system(APP "help" POST));
	REQUIRE(0 == system(APP "useless help garbage" POST));
	REQUIRE(0 != system(APP "-i-dont-exist=foo" POST));
	REQUIRE(0 != system(APP "-int=foo" POST));
	REQUIRE(0 != system(APP "-int" POST));
	REQUIRE(0 != system(APP "-int=42.1" POST));
	REQUIRE(0 != system(APP "-int =42" POST));
	REQUIRE(0 != system(APP "-int= \t 42" POST));
	REQUIRE(0 == system(APP "-int=42" POST));

	#undef POST
	#undef APP
}
