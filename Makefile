# Makefile used to:
#   1. compile the findcode program
#   2. run automated tests

# Compilation flags
CC = gcc
CFLAGS = -g -O2 -Wall
TARGET = findcode

# List of source files to include on the build
#SRCS = src/string.c
#SRCS += src/node.c
#SRCS += src/stack.c
#SRCS += src/tree.c
#SRCS += src/parser.c
SRCS += src/findcode.c

OBJS = $(SRCS:.c=.o)

# flags for the memory check phrase using valgrind
MEMCHECK_FLAGS = --track-origins=yes --leak-check=full

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
	rm -f src/*.o $(TARGET)
