/**
 * Example with actions
 */
#include "../Lib/CmdParameters.h"

using namespace std;


CmdParameters definition = {
	"Example with usage of actions\n\n"
	"Usage:\n\n"
	"    > Actions <action> {options}",

	// Actions
	{
		{
			"run",
			"This action does something cool",
			"This is the long-winded description of action 'run'. Words fail to\n"
			"describe how cool this action is.",
			{
				{
					"Num Iterations",
					"-num=",
					UNSIGNED_INTEGER,
					"The number of iterations to perform in this action"
				}
			}
		},{
			"profile",
			"Make profile output of whatever the action does",
			"This action is possibly even cooler than 'run',\n"
			"by all means give it a try!",
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
	if (ret != CmdParameters::ALL_IS_WELL) {
		return ret;
	}

	// TODO: do something with parameters

	return 0;
}
