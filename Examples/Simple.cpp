/**
 * Example with one integer switch on the command line
 */
#include <iostream>
#include "../Lib/CmdParameters.h"

using namespace std;


CmdParameters params = {
  "Simple test with single integer command line parameters\n",
  {{
    "An integer value",
    "-int=",
    INTEGER,
    "This value can be any integer"
  }}
};


int main(int argc, const char *argv[]) {
	auto ret = params.handle_commandline(argc, argv, false);
	if (ret != CmdParameters::ALL_IS_WELL) {
		return ret;
	}

	cout << "switch value: " << params.parameters()[0]->get_int_value() << endl;

	return 0;
}
