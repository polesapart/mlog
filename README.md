# MLOG -- A micro logger for constrained systems.

## Description

MLOG is a super-efficient logger for constrained systems like small embedded systems -- systems that cannot afford traditional string-based logging.

The idea is simple: instead of space-consuming log strings, 1-byte constants are defined in the code whose symbol name carries the text that is to be logged. For instance:
```
// Traditional logging:
logger("Inside function foo");

// Micro logging:
MLOG_INFO(Inside_function_foo);
```
The `MLOG_INFO` macro once statically allocates a one-byte symbol that looks more or less like this:
```
static const uint8_t MLOG_Comp_II_Inside_function_foo___;
```
Whenever execution reaches the `MLOG_INFO` macro, the address of this symbol is transferred to the log buffer. Later, on the receiving side, the original log string can be reconstructed with the help of the map file.

The benefit of the micro logging approach is clear: with traditional logging, 20 bytes would have to be stored and 20 bytes would have to be transmitted; with micro logging, only 1 byte needs to be stored and only 4 bytes need to be transmitted.

## Dependencies

For running the provided example: just `g++` and `python3`

If you want to extend/unit test MLOG itself, that is, if you want to run `make` in the top-level directory, you need [Google Test/Google Mock](https://github.com/google/googletest). Please adapt paths in Makefile accordingly.

## Portability

The code for MLOG is highly portable and should compile with any contemporary C/C++ toolchain. `generate_mlogmap.sh` currently supports extraction of MLOG log symbols from `gcc\g++` map files only.

## Usage
Have a look at the README.md file in `example/`. 

