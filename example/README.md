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

