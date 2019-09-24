# program executable name
TARGET = task_d
# compiler flags
CFLAGS = -Wall -std=gnu11 -g -O3 -lpthread -fsanitize=thread
# linker flags
LDFLAGS = -std=gnu11 -g -O3 -lpthread -fsanitize=thread
# list of sources
SOURCES = $(shell find -name "*.c")
# default rule, to compile everything
all: $(TARGET)
# define object files
OBJECTS = $(SOURCES:.c=.o)
# link programs
$(TARGET): $(OBJECTS)
	clang $(LDFLAGS) -o $@ $^
# compile
%.o : %.c
	clang $(CFLAGS) -c -o $@ $<
# cleaning
clean:
	rm -f $(TARGET) $(OBJECTS)
