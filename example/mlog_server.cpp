#include "mlog_server.h"
#include "mlog_integration.h"

#include <cstddef>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>


static const int TARGET_PORT = 12350;
static const char* TARGET_HOST = "127.0.0.1";

static int s_my_socket;
static struct sockaddr_in s_sock_adr_target;

extern "C" void mlog_transmit_server(void) {
    const mlog_uint8_t* buffer = mlog_log_buffer();
    size_t len = mlog_log_buffer_head() - buffer;
    if (sendto(s_my_socket, buffer, len, 0, (const sockaddr*) &s_sock_adr_target, sizeof(s_sock_adr_target)) == -1) {
        fprintf(stderr, "Cannot transmit data");
        exit(1);
    }
    mlog_init_log_buffer();
}

void mlog_server_init() {
    if ((s_my_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) ==-1) {
        fprintf(stderr, "Cannot create socket");
        exit(1);
    }

    memset((char *) &s_sock_adr_target, 0, sizeof(s_sock_adr_target));
    s_sock_adr_target.sin_family = AF_INET;
    s_sock_adr_target.sin_port = htons(TARGET_PORT);
    if (inet_aton(TARGET_HOST, &s_sock_adr_target.sin_addr)==0) {
        fprintf(stderr, "Cannot create socket address");
        exit(1);
    }
}

