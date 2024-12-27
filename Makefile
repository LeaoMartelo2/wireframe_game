
CC = gcc
WINDOWS_COMPILER = x86_64-w64-mingw32-gcc 

FLAGS = -Wall -Wextra -pedantic -lm -O2 -std=c17 -ggdb -L ./raylib/linux/ -lraylib
WINDOWS_FLAGS = -Wall -Wextra -pedantic -lm -O2 -std=c17 -L ./raylib/windows/ -lraylib -lgdi32 -lwinmm 

SRCDIR = src
OBJDIR = obj
WINOBJDIR = obj/win

SRC = $(wildcard $(SRCDIR)/*.c)

OBJ = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))
WINOBJ = $(patsubst $(SRCDIR)/%.c, $(WINOBJDIR)/%.o, $(SRC))

# Default target for Linux
main: $(OBJ)
	$(CC) $^ $(FLAGS) -o main

# Windows target
win: $(WINOBJ)
	$(WINDOWS_COMPILER) $^ $(WINDOWS_FLAGS) -o main.exe

# Compile object files for Linux
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(FLAGS) -c $< -o $@

# Compile object files for Windows
$(WINOBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(WINOBJDIR)
	$(WINDOWS_COMPILER) $(WINDOWS_FLAGS) -c $< -o $@

# Clean all builds
clear: 
	@rm -f $(OBJDIR)/*.o $(WINOBJDIR)/*.o main main.exe
