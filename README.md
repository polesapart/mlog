# MLOG -- A micro logger for constrained systems.

## Description

MLOG is a super-efficient logger for constrained systems like small embedded systems -- systems that cannot afford aditional string-based logging.

The idea is simple: instead of space-consuming log strings, 1-byte constants are defined whose symbol name carries the text that is to be logged. For instance:
```
// Traditional logging:
logger("Inside function foo");

// Micro logging:
MLOG_INFO(Inside_function_foo);
```
The `MLOG_INFO` macro first statically allocates a one-byte symbol that looks more or less like this:
```
static const uint8_t MLOG_Comp_II_Inside_function_foo___;
```
Whenever execution reaches the `MLOG_INFO` macro, the address of this symbol is transferred to the log buffer. Later, on the receiving side, the original log string can be reconstructed with the help of the map file.

The benefit of the micro logging approach is clear: with traditional logging, 20 bytes would have to be stored and 20 bytes would have to be transmitted; with micro logging, only 1 byte needs to be stored and only 4 bytes need to be transmitted.

## Usage
Have a look at the README.md file in `example/`. 


