# CMM Compiler
CMM Compiler is an advanced version of CMM language pack with translation program-code directly to executable file `.exe`.

## CMM Syntax

 All the syntax is written [here](https://github.com/Vokerlee/CMM-Language/blob/master/README.md). But there are some important changes:
 
 1. Instead of `double` type of every variable `int` is used.
 2. Because of usage of general registers there is no sense to keep such functions as `sin`, ..., `ln`, because they work only with `double` values. So all trigonometric, hyperbolic functions are removed, `ln` is removed. 
 3. At the same time the only expression type in `deriv` function is a polynomial, that's why this function is removed too because of its diminishing its meaning.
 4. Operator `^` and function `power` are removed, because CPU doesn't support power instructions.

Despite the fact that there are a lot of changes, CMM is still full-Turing language.

## General functionality

With considering all  changes in syntax `frontend`, `optimizer` and `reversed frontend` are used in the same way, as in [old CMM version](https://github.com/Vokerlee/CMM-Language/blob/master/README.md).

## Backend

This part of CMM language is worth separate consideration, because there are a lot of changes. 

First of all, now there is no such strata between program and executable file as assembly code. Now all CMM program compiles directly into `.exe` file. Also executable code is analyzed by CPU directly, without any interaction between other program, such as `nCPU` like in old version. 

The spacing of the program is always `32 bit`.

Functions `print` and `scan` are supported by `sfasmlib.dll` library (Simple and Fast Assembler Library), which imports `PrintNumber` and `GetNumber` functions and uses `kernel32.lib`. Also this library imports `ExitProgram`, which calls `ExitProcess` function.

Backend call:
```batch
"backend.exe" program.tree program.exe
```

## General boost

On the average now CMM compiled program executes 2500 times faster.
