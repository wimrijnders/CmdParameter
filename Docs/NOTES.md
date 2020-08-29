## TODO

- Central location for the version number. Now used in:
  * README.md

### Validation

- Options must be unique within global (**DONE**) and action
- Options must be unique over any combination of global and action
- No whitespace in options and actions


### Command line parsing

- **DONE** Action-specific help - show options per action (+ long blurb)
- Handle `UNNAMED` for actions, also in combination with global `UNNAMED`
- If any action defined, action *must* be used on command line


### Code

- Get tabs/spaces right for indent -> 2 spaces
- Use eastern const

-----

## Release Notes

```
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
    fix(static lib) : Hide symbols in static library
    fix(validation) : Moved definition validations to separate class
    fix(validation) : Return stream buffer from `add_error/add_warning`
    fix(validation) : Error messages are collected in a list, rather than output directly to stdout


20181222  v0.2.0  Enabled usage of multiple switches for a parameter
                  Used to define '-h' and 'help' to do the same thing.

20181222  v0.1.1  Fix compile error with `isnan`.
                  Small changes in examples.
                  Update docs.
                  Actions displayed if expected and none passed.

20180714  v0.1.0  Split typed parameter handling and defined parameters into classes`TypedParameter`
                  and `CmdParameters`.
                  First tentative version of actions.
                      
20180711  v0.0.1  Initial revision
```
