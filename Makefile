# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -O2 -fPIE  # Use -fPIE to compile position-independent code
LDFLAGS = -lncurses -fPIE         # Link with PIE support

# Source and Object files
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

# Output Executable
EXEC = snake

# Default target: build the executable
all: $(EXEC)

# Link object files into the executable
$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LDFLAGS)

# Compile each .c file to .o object file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJ) $(EXEC)

# Run the program
run: $(EXEC)
	./$(EXEC)

# Phony targets
.PHONY: all clean run
