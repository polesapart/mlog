#include "gtest/gtest.h"

#define MLOG_OPT
#define MLOG_TAG MlogTest

#include "mlog_base.h"
#include "mlog_integration.h"

// Best practice: put tests into a namespace
namespace testing { 
namespace mlog {

/////////////////////////////////////////////////////////////////////
// Google Test.
//

enum SEARCH_MODE {
    SEARCH_MODE_FORWARD,
    SEARCH_MODE_BACKWARD
};

inline mlog_uint8_t* toText(mlog_uint32_t adr) {
    return reinterpret_cast<mlog_uint8_t*>(adr);
}

static const mlog_uint8_t* getFirstLogEntry();
static bool getNextLogEntry(bool reset, mlog_uint32_t* address, bool* hasValue, mlog_value_t* value);
static bool advancedCharSearch(const char* text, char ch, SEARCH_MODE searchMode);

TEST(MlogTest, Simple) {
    mlog_init();
    mlog_log(toText(0x00123456));
    const mlog_uint8_t* logBuffer = getFirstLogEntry();
    EXPECT_EQ(0x00, logBuffer[0]);
    EXPECT_EQ(0x12, logBuffer[1]);
    EXPECT_EQ(0x34, logBuffer[2]);
    EXPECT_EQ(0x56, logBuffer[3]);

    MLOG_INFO(Hello_there);
}

TEST(MLogTest, SuccessiveTexts) {
    mlog_init();
    const mlog_uint8_t* buf = mlog_log_buffer();

    mlog_log(toText(0x00778899));
    mlog_log(toText(0x00ABCDEF));

    mlog_uint32_t adr;
    mlog_value_t val;
    bool hasVal;
    bool success;

    success = getNextLogEntry(true, &adr, &hasVal, &val);
    EXPECT_TRUE(success);
    EXPECT_EQ(0x00778899, adr);
    EXPECT_FALSE(hasVal);

    success = getNextLogEntry(false, &adr, &hasVal, &val);
    EXPECT_TRUE(success);
    EXPECT_EQ(0x00ABCDEF, adr);
    EXPECT_FALSE(hasVal);

    // Attempt to read beyond buffer.
    success = getNextLogEntry(false, &adr, &hasVal, &val);
    EXPECT_FALSE(success);
} 

TEST(MLogTest, SuccessiveTextsAndValues) {
    mlog_init();

    mlog_log(toText(0x00778899));
    mlog_log_value(toText(0x00ABCDEF), 12345);
    mlog_log(toText(0x00457799));
    mlog_log_value(toText(0x00B0B1B2), 987654321);

    mlog_uint32_t adr;
    mlog_value_t val;
    bool hasVal;
    bool success;

    success = getNextLogEntry(true, &adr, &hasVal, &val);
    EXPECT_EQ(0x00778899, adr);
    EXPECT_FALSE(hasVal);

    success = getNextLogEntry(false, &adr, &hasVal, &val);
    EXPECT_EQ(0x00ABCDEF, adr);
    EXPECT_EQ(12345, val);

    success = getNextLogEntry(false, &adr, &hasVal, &val);
    EXPECT_EQ(0x00457799, adr);
    EXPECT_FALSE(hasVal);

    success = getNextLogEntry(false, &adr, &hasVal, &val);
    EXPECT_EQ(0x00B0B1B2, adr);
    EXPECT_EQ(987654321, val);
} 

TEST(MLogTest, ValuesOtherThanUnit32) {
    mlog_init();

    // 1. No value passed at al.
    MLOG_INFO(without_value);
    // 2. Pass an int.
    MLOG_INFO_VALUE(with_value, 1234);
    // 3. Pass an enum.
    enum colors_e { red = 42, green = 43, blue = 44 };
    colors_e current_color = green;
    MLOG_INFO_VALUE(Current_color, current_color);
    // 4. Pass double.
    MLOG_INFO_VALUE(pi, 3.1415);

    mlog_uint32_t adr;
    mlog_value_t val;
    bool hasVal;
    bool success;

    // 1.
    success = getNextLogEntry(true, &adr, &hasVal, &val);
    EXPECT_FALSE(hasVal);
    // 2.
    success = getNextLogEntry(false, &adr, &hasVal, &val);
    EXPECT_TRUE(hasVal);
    EXPECT_EQ(1234, val);
    // 3.
    success = getNextLogEntry(false, &adr, &hasVal, &val);
    EXPECT_TRUE(hasVal);
    EXPECT_EQ(43, val);
    // 4.
    success = getNextLogEntry(false, &adr, &hasVal, &val);
    EXPECT_TRUE(hasVal);
    EXPECT_EQ(3, val);
}

TEST(MLogTest, AdvancedCharSearch) {
    EXPECT_TRUE(advancedCharSearch("Foobar", 'o', SEARCH_MODE_FORWARD));
}

static const mlog_uint8_t* getFirstLogEntry() {
    return mlog_log_buffer() + sizeof(MLOG_MAGIC_HEADER);
}

static bool getNextLogEntry(bool reset, mlog_uint32_t* address, bool* hasValue, mlog_value_t* value) {
    static const mlog_uint8_t* s_current;

    if (reset) {
        s_current = getFirstLogEntry();

    }

    // If end of buffer reached.
    if (s_current + sizeof(mlog_uint32_t) > mlog_log_buffer_head()) {
        return false;
    }

    // Get address.
    mlog_uint32_t adr = MLOG_PEEK_MLOG_UINT32_T_BIG(s_current);
    s_current += sizeof(adr);

    if (adr & MLOG_VALUE_PRESENT_UINT32_MASK) {
        adr &= ~MLOG_VALUE_PRESENT_UINT32_MASK;
        mlog_value_t val = MLOG_PEEK_MLOG_UINT32_T_BIG(s_current);
        s_current += sizeof(val);
        *value = val;
        *hasValue = true;
    } else {
        *hasValue = false;
    }
    
    *address = adr;

    return true;
}

static bool advancedCharSearch(const char* text, char ch, SEARCH_MODE searchMode) {
    bool found = false;

    MLOG_INFO(Enter_AdvancedCharSearch);

    MLOG_ENSURE(text != 0, text_must_not_be_null);

    if (ch == '\0') {
        MLOG_WARN(search_char_should_not_be_null);
    }

    int delta;
    const char* begin;
    const char* end;

    switch (searchMode) {
    case SEARCH_MODE_FORWARD:
        begin = text;
        end = text + strlen(text) - 1;
        delta = 1;
        break;
    case SEARCH_MODE_BACKWARD:
        begin = text + strlen(text) - 1;
        end = text;
        delta = -1;
    default:
        MLOG_ERROR_VALUE(invalid_search_mode, searchMode); 
        return false;
    }

    for (;;) {
        if (*begin == ch) {
            found = true;
            break;
        }
        if (begin == end) {
            break;
        }

        begin += delta;
        end += delta;
    }

    MLOG_INFO(Exit_AdvancedCharSearch);
    return found;
}


}
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

