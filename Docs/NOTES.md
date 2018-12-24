## TODO

- Replace `#ifdef LITE` with something better
  * currently, usage is invalidated
  * better to make 'LITE' compile output in some way

- Central location for the version number. Now used in:
  * README.md
  * Lite/gen_lite.rb

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

2018/12/21  v0.2.0  Enabled usage of multiple switches for a parameter
                    Used to define '-h' and 'help' to do the same thing.

2018/12/21  v0.1.1  Fix compile error is `isnan`.
                    Small changes in examples.
                    Update docs.
                    Actions displayed if expected and none passed.

2018/07/14  v0.1.0  Split typed parameter handling and defined parameters into classes`TypedParameter`
                    and `CmdParameters`.
                    First tentative version of actions.
                      
2018/07/11  v0.0.1  Initial revision
```
