/**
 * Example with one integer switch on the command line
 */
#include <iostream>
#include "../Lib/CmdParameter.h"

using namespace std;


static const char *usage =
"Simple test with single integer command line parameters\n"
;


DefParameter defined_parameters[] = {
  {
    "An integer value",
    "-int=",
    INTEGER,
    "This value can be any integer"
  },
  nullptr   // End marker
};


int main(int argc, const char *argv[]) {
	auto ret = CmdParameter::handle_commandline(usage, defined_parameters, argc, argv, false);
	if (ret != CmdParameter::ALL_IS_WELL) {
		return ret;
	}

	cout << "switch value: " << CmdParameter::parameters[0]->get_int_value() << endl;

	return 0;
}
