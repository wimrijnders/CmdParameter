CmdParameter
============

Version 0.2.0

`CmdParameter` is a library for handling command line parameters in C++.

Its goals are:

  - make it as simple as feasible to define parameters and use them
  - make it easy to add new parameter types
  - define command definitions (internally called 'actions')

 Note that these are very different goals from making this simple.
 The downside is that this code becomes more complex. I think this is a worthwhile tradeoff.
 
To be more specific:

 - **Declarative definitions** - The command line parameters are defined in a structure separate from program code.
 - **Allow commands** - Commands of the form `app command {options}` are allowed and handled
 - **Extensive validation and checking of values** - ensure that anything will be correct upon usage, in an extensible way.
 - **Arbitrarily large help text** - start small, add more as required without limit.

-----

## Example

This definition, taken from the `Simple` example program:

```c++
CmdParameters parames = {
  "Simple test with single integer command line parameters\n",
  {{
    "An integer value",
    "-int=",
    INTEGER,
    "This value can be any integer"
  }}
};
};
```

Leads to the following output:

```
> ./Simple -h
Simple test with single integer command line parameters

Options:

    -h          Show this information. Overrides all other parameters.
    -int=<num>  This value can be any integer; default '0'.

Notes:

 * Global options can appear in any position on the command line after the program name.

> ./Simple -int=hello
Error(s) on command line:
  The value for field 'An integer value' is not a number.

  Use switch '-h' to view options

> ./Simple -int
Error(s) on command line:
  Parameter 'An integer value' (-int) takes a value, none specified.

  Use switch '-h' to view options

> ./Simple -int=
Error(s) on command line:
  Parameter 'An integer value' (-int) takes a value, none specified.

  Use switch '-h' to view options

> ./Simple -int=42
switch value: 42

```

-----

## TODO

- Allow concatenated short options (no parameters), e.g. `-AaBb`
