## Building the example
In a terminal do this:
```
cd <mlog-dir>/example
make
```
After running `make` you should have an executable called `example` and an mlogmap file called `example.nmaplog` in the `example` directory.

## Starting the logger
Start another terminal:
```
cd <mlog-dir>/example/mlog_logger
./mlog_logger.py ../example.nmaplog
```

## Running the example
Go to the first terminal and launch the `example` application:
```
./example
```
You should see the log output of `example` in the second terminal (the one where you started `mlog_logger`).

```
2.469 Example INFO Startup
2.469 Example INFO Starting_prime_number_computation
2.469 Example INFO Prime_number 0x2 2
2.469 Example INFO Prime_number 0x3 3
2.469 Example INFO Prime_number 0x5 5
2.469 Example INFO Prime_number 0x7 7
2.469 Example INFO Prime_number 0xB 11
:
6.777 Example INFO Prime_number 0x3D1 977
6.777 Example INFO Prime_number 0x3D7 983
6.777 Example INFO Prime_number 0x3DF 991
6.777 Example INFO Prime_number 0x3E5 997
:
6.777 Example INFO Exiting
```
