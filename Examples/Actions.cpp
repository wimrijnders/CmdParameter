/**
 * Example with actions
 */
#include "../Lib/CmdDefinition.h"

using namespace std;


CmdDefinition definition = {
	"Example with usage of actions\n\n"
	"Usage:\n\n"
	"    > Actions <action> {options}",

	// Actions
	{
		{
			"run",
			"This action does something cool",
			{
				{
					"Num Iterations",
					"-num=",
					INTEGER,
					"The number of iterations to perform in this action"
				}
			}
		},{
			"profile",
			"Make profile output of whatever the action does",
			{
				{
					"Output json",
					"-json",
					NONE,
					"Output the profile info in json format"
				}
			}
		}
	},

	// Global options
	{
		{
			"Quiet mode",
			"-quiet",
			NONE,
			"Don't show any extra output while running"
		}
	}
};


int main(int argc, const char *argv[]) {
	auto ret = definition.handle_commandline(argc, argv, false);
	if (ret != CmdDefinition::ALL_IS_WELL) {
		return ret;
	}

	// TODO: do something with parameters

	return 0;
}
