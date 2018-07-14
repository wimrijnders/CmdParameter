/**
 * Example with one integer switch on the command line
 */
#include <iostream>
#include "../Lib/CmdDefinition.h"

using namespace std;


CmdDefinition definition = {
  "Simple test with single integer command line parameters\n",
  {{
    "An integer value",
    "-int=",
    INTEGER,
    "This value can be any integer"
  }}
};


int main(int argc, const char *argv[]) {
	auto ret = definition.handle_commandline(argc, argv, false);
	if (ret != CmdDefinition::ALL_IS_WELL) {
		return ret;
	}

	cout << "switch value: " << definition.parameters()[0]->get_int_value() << endl;

	return 0;
}
