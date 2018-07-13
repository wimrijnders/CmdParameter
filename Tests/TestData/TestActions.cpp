#include "TestActions.h"
//#include "CmdParameter.h"


CmdDefinition defined_actions {
  "Blurb",

  {{
    "action1",
    "This is action 1", {{
      "Unsigned integer param",
      "-unsigned=",
      UNSIGNED_INTEGER,
      "This is a parameter specific to action 'Action1'"
    }}
  }, {
    "action2",   // Has no parameters, should be allowed
    "This is action 2"
  }, {
    "action3",
    "This is action 3", {{
      "Another unsigned integer param",  // One same parameter as Action1, should be allowed
      "-unsigned=",
      UNSIGNED_INTEGER,
      "This is a parameter specific to action 'Action2'"
    }, {
      "A string param",
      "-str=",
      STRING,
      "This is a string parameter specific to action 'Action3'"
     }
  }}
}};
