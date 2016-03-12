TARGET = example

MLOG_DIR = ..
CPPFLAGS += -g3 -pthread -I $(MLOG_DIR)/include

$(TARGET).mlogmap: $(TARGET)
	@$(MLOG_DIR)/generate_mlogmap.sh $(TARGET) > $(TARGET).mlogmap

$(TARGET): $(TARGET).cpp mlog_server.cpp ../src/mlog.c

.PHONY :  clean
clean:
	rm -rf *.o $(TARGET) $(TARGET).exe $(TARGET).mlogmap

