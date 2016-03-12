#include "mlog_base.h"
#include "mlog_integration.h"
#include <stddef.h>

#ifdef MLOG_DEBUG_OUTPUT
#include <stdio.h>
#endif

mlog_uint8_t g_mlog_buffer[MLOG_LOG_BUFFER_LENGTH];
const static mlog_uint8_t* const MLOG_LOGBUF_END = g_mlog_buffer + sizeof(g_mlog_buffer);

mlog_uint8_t* g_mlog_buffer_head;
mlog_uint8_t g_mlog_initialized = 0;

void mlog_init(void) {
    mlog_init_log_buffer();
    g_mlog_initialized = 1;
}

void mlog_shutdown(void) {
    mlog_transmit_server();
    g_mlog_initialized = 0;
}

void mlog_log(const volatile mlog_uint8_t* text) {
    if (g_mlog_initialized != 0) {
#ifdef MLOG_DEBUG_OUTPUT
        printf("-----------> mlog_log(%x)\n", (mlog_uint32_t) (unsigned long) text);
#endif
        mlog_uint32_t adr = (mlog_uint32_t) (unsigned long) text;

        /* Check against worst case. */
        if ((g_mlog_buffer_head + sizeof(adr)) > MLOG_LOGBUF_END) {
            mlog_transmit_server();
        }
        MLOG_STORE_MLOG_UINT32_T_BIG(g_mlog_buffer_head, adr);
    }
}

void mlog_log_value(const volatile mlog_uint8_t* text, mlog_value_t value) { 
    if (g_mlog_initialized != 0) {
#ifdef MLOG_DEBUG_OUTPUT
        printf("-----------> mlog_log_value(%x, %x)\n", (mlog_uint32_t) (unsigned long) text, value);
#endif
        mlog_uint32_t adr = (mlog_uint32_t) (unsigned long) text;

        /* Check against worst case. */
        if ((g_mlog_buffer_head + sizeof(adr) + sizeof(value)) > MLOG_LOGBUF_END) {
            mlog_transmit_server();
        }

        adr |= MLOG_VALUE_PRESENT_UINT32_MASK;

        MLOG_STORE_MLOG_UINT32_T_BIG(g_mlog_buffer_head, adr);
        MLOG_STORE_MLOG_UINT32_T_BIG(g_mlog_buffer_head, value);
    }
}

void mlog_init_log_buffer(void) {
    size_t i;
    g_mlog_buffer_head = g_mlog_buffer;
    for (i = 0; i < sizeof(MLOG_MAGIC_HEADER); ++i) {
        *g_mlog_buffer_head++ = (mlog_uint8_t) MLOG_MAGIC_HEADER[i];
    }
}

#ifdef MLOG_TRANSMIT_DUMMY
void mlog_transmit_server(void) {
    mlog_init_log_buffer();
}
#endif

