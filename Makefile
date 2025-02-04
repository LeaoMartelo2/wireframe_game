.PHONY: clear
CC = g++
WINDOWS_COMPILER = x86_64-w64-mingw32-g++

DISABLED_WARNINGS = -Wno-missing-field-initializers -Wno-format-overflow 

FLAGS = -std=c++20 -Wall -Wextra $(DISABLED_WARNINGS) -pedantic -lm -O3

POSIX_FLAGS = -L ./raylib/linux/ -lraylib
WINDOWS_FLAGS = -L ./raylib/windows/ -lraylib -lgdi32 -lwinmm -lopengl32 -static -mwindows

SRCDIR = src
OBJDIR = build/posix
WINOBJDIR = build/win32

SRC = $(wildcard $(SRCDIR)/*.cpp)

OBJ = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC))
WINOBJ = $(patsubst $(SRCDIR)/%.cpp, $(WINOBJDIR)/%.o, $(SRC))

# Default target for Linux
main: $(OBJ)
	$(CC) $^ $(FLAGS) $(POSIX_FLAGS) -o wireframe

# Windows target
win: $(WINOBJ)
	$(WINDOWS_COMPILER) $^ $(FLAGS) $(WINDOWS_FLAGS) -o wireframe.exe

# Compile object files for Linux
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CC) $(FLAGS) $(POSIX_FLAGS) -c $< -o $@

# Compile object files for Windows
$(WINOBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(WINOBJDIR)
	$(WINDOWS_COMPILER) $(FLAGS) $(WINDOWS_FLAGS) -c $< -o $@

# Clean all builds
clear: 
	@rm -f $(OBJDIR)/*.o $(WINOBJDIR)/*.o wireframe wireframe.exe 
