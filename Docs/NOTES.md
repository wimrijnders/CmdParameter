## TODO

*<plenty will be added here>*

### Validation

- Options must be unique within global (**DONE**) and action
- Options must be unique over any combination of global and action
- No whitespace in options and actions

### Command line parsing

- **DONE** Action-specific help - show options per action (+ long blurb)
- Handle `UNNAMED` for actions, also in combination with global `UNNAMED`

### Test

- **DONE** Multiple actions on command line not allowed

### Code

- Get tabs/spaces right for indent -> 2 spaces

-----

## Release Notes

```
2018/07/14  v0.1.0.0  Split typed parameter handling and defined parameters into classes`TypedParameter`
                      and `CmdParameters`.
                      First tentative version of actions.
                      
2018/07/11  v0.0.1.0  Initial revision
```
