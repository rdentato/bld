# bld

## Introduction
This project is to test an idea by Kartik Agaram: using the shell as a substitute of a build system (i.e. make, cmake, jam, ...).

You can find his post here: https://git.sr.ht/~akkartik/basic-build

The key idea is to define a function `older_then()` that checks whether a file is older than its dependecies and use it to execute the instruction to re-build it only when needed.

I've stressed his idea a little bit and decided to use bash (his suggestion was to use the Bourne shell for the maximum compatibility).

## Project structure
The assumption is that a project is stored in a directory structure similar to this (with possibly sub-dirs):

     top
      |-- src
      |-- doc
      |-- test
      |-- lib
      |-- ...
      |
      
The top directory must contain 2 scripts:

  - `bld.top` with the global variables and common functions for the projects
  - `bld.trg` with the definition of the targets that can be built from the top directory.
  
all the other directories can contain a `bld.trg` that defines the target that can be built starting from that directory.

A *target* is a function whose name starts with an underscore. For example:

    _clean () {
       # Clean up commands here.
    }
    
## Command line options
To build a target, you invoke the `bld` script.

    bld [-h] [-d dirname] [-f trgfile] [-l | target]

 Assuming `bld` is in your path, the command:

```
    > bld clean
```

will execute the `_clean` function (assuming it's defined in your `bld.trg` script). You can specify more than one target.
To build a target that is in another directory you can use the `-d` switch. For example:

```
    > bld -d test clean
```

will build the `clean` target as defined in the `test` directory.

Note that targets are just labels, they are not directly related to files.

```
    > bld -l
```
will provide the list of available targets.

## Functions

 - `bld [-d dir] [-f] [target ...]`
   Builds the specified targets.
   If the `-d` flag is specified, the build is started in the specified directory
   as a new process.
       
 - `bld_old targ f1 f2 ...`
    Checks if the target file `targ` is older than its dependencoes `f1`, `f2`, ...
    This function is to be used as in the following example:
       
            bld_old myexe src1.c src2.c && {
              # recompile myexe
            }
    
 - `bld_cmd`
   This is useful if you want the build script to emit the command that is to be executed.
   For example:
      
           bld_cmd gcc -c src.c
           
   will print on stderr the command before executing it
    
## Pseudo target
Is sometimes desirable to have *pseudo*targets, i.e. targets that can be handled in a common way.

You can define the function `_default()` that will handle any unknown target.

An example of pseudo targets is provided in the test directory of the *lorem Ipsum* project (see next section)


## The *lorem Ipsum* project

To provide a better example of how to use `bld` you can refer to the files in this repository.

The directory `src` contains the source files for a (fake) library that will be built in the `lib` directory.

The directory `test` contains the unit tests for the library. To ease the addition of new tests, the convention is that any C source file whose name starts with `ut_*` is a unit test and that any unit test itself is a program whose name starts with `t_*`

Building everything:

        > bld all
        
Checking if the `liblorem.a` library is recompiled properly

        > touch src/dolor.c
        > bld -d lib dolor
        
 
