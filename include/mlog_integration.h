#ifndef MLOG_INTEGRATION_H
#define MLOG_INTEGRATION_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mlog_types.h"

/* Target-specific adaptations . */

/* If defined, integers can be stored at any address in memory. */
#undef MLOG_BIG_ENDIAN_UNALIGNED_ACCESS

/* A really small buffer for demonstration purposes only. */
#define MLOG_LOG_BUFFER_LENGTH 100U

#define MLOG_VALUE_PRESENT_UINT8_MASK    (1U << 6U)

#define MLOG_VALUE_PRESENT_UINT32_MASK   ((MLOG_VALUE_PRESENT_UINT8_MASK) << 24U)

#ifdef MLOG_BIG_ENDIAN_UNALIGNED_ACCESS
#define MLOG_STORE_MLOG_UINT32_T_BIG(p, val) \
    do {\
        *((mlog_uint32_t*) p) = val;\
        p += sizeof(val);\
    } while (0)
#else
#define MLOG_STORE_MLOG_UINT32_T_BIG(p, val) \
    do {\
        *p++ = (mlog_uint8_t) (val >> 24);\
        *p++ = (mlog_uint8_t) (val >> 16);\
        *p++ = (mlog_uint8_t) (val >> 8);\
        *p++ = (mlog_uint8_t) (val);\
    } while (0)
#define MLOG_PEEK_MLOG_UINT32_T_BIG(p) \
    p[0] << 24 | p[1] << 16 | p[2] << 8 | p[3]
#endif

#define MLOG_FORMAT_VERSION '1'
static const char MLOG_MAGIC_HEADER[] = { 'M', 'L', 'O', 'G', MLOG_FORMAT_VERSION , ':' };

extern mlog_uint8_t g_mlog_buffer[MLOG_LOG_BUFFER_LENGTH];
extern mlog_uint8_t* g_mlog_buffer_head;

/**
 * Initialize mlog system.
 * Any log statements prior to a call to this function will be ignored.
 */
void mlog_init(void);

/**
 * Shuts down mlog system.
 * Any log statements after a call to this function will be ignored.
 */
void mlog_shutdown(void);

/**
 * Initialize (resets) the log buffer.
 */
void mlog_init_log_buffer(void);

/**
 * Callback that is called back when the server shall transmit the current
 * contents of the log buffer.
 */
void mlog_transmit_server(void);

/**
 * Returns pointer to begin of log buffer.
 */
#define mlog_log_buffer() ((const mlog_uint8_t*) g_mlog_buffer)

/**
 * Returns current head pointer.
 */
#define mlog_log_buffer_head() ((const mlog_uint8_t*) g_mlog_buffer_head)

#ifdef __cplusplus
}
#endif

#endif
