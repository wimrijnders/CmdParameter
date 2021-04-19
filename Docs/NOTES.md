## TODO

### Validation

- [x] Options must be unique within global
- [ ] Options must be unique within action
- [ ] Options must be unique over any combination of global and action
- [ ] No whitespace in options and actions
- [ ] Check correct usage assign ('=') postfix in prefixes
  * '=' not allowed for NONE
  * '=' required for integers (etc.)
  * check all prefixes in >1 list 


### Command line parsing

- [x] Action-specific help - show options per action (+ long blurb)
- [ ] Handle `UNNAMED` for actions, also in combination with global `UNNAMED`
- [ ] If any action defined, action *must* be used on command line


### Code

- [x] Get tabs/spaces right for indent -> 2 spaces
- [ ] Use eastern const


-----

## Release Notes

```
20210418  v0.3.2
    - Params sorted on display help usage
    - Added operator<< for string (from V3DLib)
    - Got rid of most usages of ostringstream


20210417  v0.3.1
    - Help usage now returned as string value
    - Also Setting struct and unit test for param sorting (not done yet).
    - Replaced CATCH2 with DocTest in unit test


20210125  v0.3.0
    - Allow multiple prefixes for options, definition as vector.
      This is an API change.
    - Fix stupid bug: `!strcmp()` - '!' to test match (idiot)
    - Converted all tab indents to 2 spaces


20201102  v0.2.6
    - Fix on parameter parsing; match should be exact when parameter
      takes no values.

20200904  v0.2.5
    - Added proper Exception class, for proper display of message
      when they get thrown.


20200831  v0.2.4
    - Added method `CmdParameters::add()`.
      Can now dynamically append parameter lists to an existing one.
      Chaining lists can now be declared redundant.


20200829  v0.2.3
    - Fixed setting of defaults for integer parameters
    - Some code cleanup and refactoring, making class `TypedParameter` simpler


20200828  v0.2.2
    - Removed 'LITE' build option, it was silly anyway
    - Added chained dependencies. Parameters can now 'inherit' another list, this is not
      really inherited by merged into the current list
    - Cleaned up resetting of defaults for parameters
    - Fixed dependencies in makefile for unit tests, some code cleanup
    - Added silent option to class `CmdParameter`, to eliminate output during unit tests
    - Code cleanup, especially debug code
 

20190103  v0.2.1
    - Hide symbols in static library
    - Moved definition validations to separate class
    - Return stream buffer from `add_error/add_warning`
    - Error messages are collected in a list, rather than output directly to stdout


20181222  v0.2.0
    - Enabled usage of multiple switches for a parameter
      Used to define '-h' and 'help' to do the same thing.


20181222  v0.1.1
    - Fix compile error with `isnan`.
    - Small changes in examples.
    - Update docs.
    - Actions displayed if expected and none passed.


20180714  v0.1.0
    - Split typed parameter handling and defined parameters into classes`TypedParameter`
      and `CmdParameters`.
    - First tentative version of actions.

                      
20180711  v0.0.1
    - Initial revision
```
