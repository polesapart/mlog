#ifndef MLOG_BASE_H
#define MLOG_BASE_H

#include "mlog_types.h"

/**
 * NOTE!
 * You can include this header directly but you shouldn't! Instead, create a
 * local mlog.h file for your component, define an apropriate log tag, then
 * include your local mlog.h file instead.
 **/

#ifndef MLOG_TAG
/* Default log tag if user didn't define one. */
#define MLOG_TAG Global
#endif

/* Log text 'm' at level 'info'. */
#define MLOG_INFO(m)                MLOGMLOG_WITH_TAG_(II_##m)
/* Log text 'm' at level 'warning'. */
#define MLOG_WARN(m)                MLOGMLOG_WITH_TAG_(WW_##m)
/* Log text 'm' at level 'error'. */
#define MLOG_ERROR(m)               MLOGMLOG_WITH_TAG_(EE_##m)

/* Log text 'm' and mlog_log_value 'v' at level 'info'. */
#define MLOG_INFO_VALUE(m, v)       MLOG_VALUE_WITH_TAG_(II_##m, v)
/* Log text 'm' and mlog_log_value 'v' at level 'warning'. */
#define MLOG_WARN_VALUE(m, v)       MLOG_VALUE_WITH_TAG_(WW_##m, v)
/* Log text 'm' and mlog_log_value 'v' at level 'error'. */
#define MLOG_ERROR_VALUE(m, v)      MLOG_VALUE_WITH_TAG_(EE_##m, v)

/* Check condition 'e' and if false, report text 'm' at level 'error'. */
#define MLOG_ENSURE(e, m)\
do {\
    if (!e) { MLOG_ERROR(ENSURE_##m); }\
} while (0)

/* Just implementation details below. */

#define MLOGMLOG_WITH_TAG_(m)        MLOG_(MLOG_TAG, m)
#define MLOG_VALUE_WITH_TAG_(m, v)   MLOG_VALUE_(MLOG_TAG, m, v)
    
#define MLOG_(t, m)                 MLOG__(t, m)
#define MLOG_VALUE_(t, m, v)        MLOG_VALUE__(t, m, v)

#define MLOG_(t, m)                 MLOG__(t, m)
#define MLOG_VALUE_(t, m, v)        MLOG_VALUE__(t, m, v)

#define MLOG__(t, m)\
do {\
    static const volatile mlog_uint8_t MLOG_##t##_##m##___ = 0;\
    mlog_log(&MLOG_##t##_##m##___);\
} while (0)
#define MLOG_VALUE__(t, m, v)\
do {\
    static const volatile mlog_uint8_t MLOG_##t##_##m##___ = 0;\
    mlog_log_value(&MLOG_##t##_##m##___, v);\
} while (0)

#ifdef __cplusplus
extern "C" {
#endif

void mlog_log(const volatile mlog_uint8_t* text);
void mlog_log_value(const volatile mlog_uint8_t* text, mlog_value_t value);

#ifdef __cplusplus
}
#endif

#endif

