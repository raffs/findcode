# Makefile used to:
#   1. compile the findcode program
#   2. run automated tests

# Compilation flags
CC = gcc
CFLAGS = -g -O2 -Wall
TARGET = findcode

# List of source files to include on the build
SRCS = src/findcode.c
SRCS += src/node.c
SRCS += src/stack.c
SRCS += src/string.c

OBJS = $(SRCS:.c=.o)

# flags for the memory check phrase using valgrind
MEMCHECK_FLAGS = --track-origin=yes --leak-check=full

.PHONY: clean

# ---------------------------------------------------------
#  makefile rules
# ---------------------------------------------------------

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

memcheck: $(TARGET)
	@valgrind $(MEMCHECK_FLAGS) ./$(TARGET)

clean:
	@echo cleaning up the files
