#include "mlog.h"
#include "mlog_integration.h"
#include "mlog_server.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    MLOG_ENSURE(argc == 1, Please_do_not_provide_arguments_to_main);

    mlog_server_init();
    mlog_init();
    MLOG_INFO(Startup);

    // No buffering for standard output.
    setvbuf(stdout, 0, _IONBF, 0);

    MLOG_INFO(Starting_prime_number_computation);

    // A poor algorithm to find prime numbers.
    for (int i = 2; i < 1000; ++i) {
        int candidate = i;
        for (int j = 2; j < candidate - 1; ++j) {
            // Slow it down a little...
            usleep(0);
            if (candidate % j == 0) {
                candidate = 0;
                break;
            }
        }
        if (candidate > 0) { 
            MLOG_INFO_VALUE(Prime_number, candidate);
        }
    }

    MLOG_INFO(Exiting);
    mlog_shutdown();

    printf("\n");

    return 0;
}

