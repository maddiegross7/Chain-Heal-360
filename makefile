# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra

# Output binary
TARGET = chain_heal

# Source file
SRC = chain_heal.c

# Libraries
LIBS = -lm

# Default rule
all: $(TARGET)

# Rule to build the target
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LIBS)

# Clean rule to remove compiled files
clean:
	rm -f $(TARGET)
