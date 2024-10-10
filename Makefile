# Makefile for Terra Text Editor

# Compiler
CC = gcc

# Compiler flags for warnings and debugging
CFLAGS = -Wall -Wextra -g

# Source files
SRC = src/main.c
HD = src/headers/utilities.c

# Include directories for header files
INC = -I./src/headers

# Output executable
OUTPUT = ./dist/terra_editor

# Object files
OBJ = $(SRC:.c=.o) $(HD:.c=.o)

# Default target (build the executable)
all: $(OUTPUT)

# Rule to build the executable
$(OUTPUT): $(OBJ)
	$(CC) $(OBJ) -o $(OUTPUT)


# Rule to compile each .c file into .o files
%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

# Clean rule to remove generated files
clean:
	rm -f $(OBJ) $(OUTPUT)

# Phony targets to avoid confusion with file names
.PHONY: all clean
