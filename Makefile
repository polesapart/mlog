# 
# gmock also contains gtest!
#

TARGET = test_mlog

GMOCK_PATH = #### ~/gmock
GTEST_PATH = $(GMOCK_PATH)/gtest

CPPFLAGS += -DMLOG_TRANSMIT_DUMMY -DMLOG_DEBUG_OUTPUT
CPPFLAGS += -g3 -pthread -I $(GTEST_PATH)/include -I include

LDFLAGS += -L $(GMOCK_PATH)
LDLIBS += -l gmock

$(TARGET): $(TARGET).cpp src/mlog.c

.PHONY : test
test : $(TARGET)
	./$<

.PHONY : mlogmap
mlogmap: 
	@./generate_mlogmap.sh $(TARGET) > $(TARGET).mlogmap
	
.PHONY :  clean
clean:
	rm -rf *.o $(TARGET) $(TARGET).exe $(TARGET).mlogmap


