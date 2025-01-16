CC = g++
WINDOWS_COMPILER = x86_64-w64-mingw32-g++

FLAGS = -Wall -Wextra -Wno-missing-field-initializers -Wno-format-overflow -pedantic -lm -O2 -std=c++23 -L ./raylib/linux/ -lraylib
WINDOWS_FLAGS = -Wall -Wextra -Wno-missing-field-initializers -Wno format-overflow -pedantic -lm -O2 -L ./raylib/windows/ -lraylib -lgdi32 -lwinmm -lopengl32 -static

SRCDIR = src
OBJDIR = obj
WINOBJDIR = obj/win

SRC = $(wildcard $(SRCDIR)/*.cpp)

OBJ = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC))
WINOBJ = $(patsubst $(SRCDIR)/%.cpp, $(WINOBJDIR)/%.o, $(SRC))

# Default target for Linux
main: $(OBJ)
	$(CC) $^ $(FLAGS) -o main

# Windows target
win: $(WINOBJ)
	$(WINDOWS_COMPILER) $^ $(WINDOWS_FLAGS) -o main.exe

# Compile object files for Linux
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CC) $(FLAGS) -c $< -o $@

# Compile object files for Windows
$(WINOBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(WINOBJDIR)
	$(WINDOWS_COMPILER) $(WINDOWS_FLAGS) -c $< -o $@

# Clean all builds
clear: 
	@rm -f $(OBJDIR)/*.o $(WINOBJDIR)/*.o main main.exe 
