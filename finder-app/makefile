# Compiler configuration
CROSS_COMPILE ?=
CC = $(CROSS_COMPILE)gcc

# Compiler and linker flags
CFLAGS = -Wall -Wextra -Werror -g
LDFLAGS =

# Application and source files
TARGET = writer
SRCS = writer.c
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Linking
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

# Compilation
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean generated files
clean:
	rm -f $(TARGET) *.o

.PHONY: all clean
