# Variables
CC = gcc
CFLAGS = -Wall -I.
LIB = lib/libraycast.a
OBJ = lib/misc.o lib/map.o
SRC = misc.c map.c

# Default target to build the static library
all: $(LIB)

# Compile object files
$(OBJ): $(SRC)
	$(CC) -c misc.c -o lib/misc.o
	$(CC) -c map.c -o lib/map.o

# Create static library from object files
$(LIB): $(OBJ)
	ar rcs $(LIB) $(OBJ)

# Clean up object files and the static library
clean:
	rm -f $(OBJ) $(LIB)
