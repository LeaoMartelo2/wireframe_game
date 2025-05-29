.PHONY: clear release
CC = clang++
WINDOWS_COMPILER = x86_64-w64-mingw32-g++

DISABLED_WARNINGS = -Wno-missing-field-initializers -Wno-format-overflow -Wno-unused-command-line-argument -Wno-c99-extensions -Wno-missing-braces

FLAGS = -std=c++20 -Wall -Wextra $(DISABLED_WARNINGS) -pedantic -lm -ggdb

RELEASE_FLAGS = -O2 -DLOGNEST_DISABLE_DEBUG

ifdef DEBUG
	FLAGS += -DDEBUG
endif

POSIX_FLAGS = -L ./raylib/linux/ -lraylib
WINDOWS_FLAGS = -L ./raylib/windows/ -lraylib -lgdi32 -lwinmm -lopengl32 -static -mwindows

SRCDIR = src
OBJDIR = build/posix
WINOBJDIR = build/win32

SRC = $(wildcard $(SRCDIR)/*.cpp)

OBJ = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC))
WINOBJ = $(patsubst $(SRCDIR)/%.cpp, $(WINOBJDIR)/%.o, $(SRC))

# Default target for Linux
wireframe: $(OBJ)
	$(CC) $^ $(FLAGS) $(POSIX_FLAGS) -o wireframe

win: wireframe.exe

# Windows target
wireframe.exe: $(WINOBJ)
	$(WINDOWS_COMPILER) $^ $(FLAGS) $(WINDOWS_FLAGS) -o wireframe.exe

# Compile object files for Linux
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CC) $(FLAGS) $(POSIX_FLAGS) -c $< -o $@

# Compile object files for Windows
$(WINOBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(WINOBJDIR)
	$(WINDOWS_COMPILER) $(FLAGS) $(WINDOWS_FLAGS) -c $< -o $@


release: FLAGS += ${RELEASE_FLAGS}
release: wireframe win
	@mkdir -p release/Linux-x64
	@mkdir -p release/Windows-x64

	@cp wireframe release/Linux-x64
	@cp wireframe.exe release/Windows-x64

	@cp -r levels release/Linux-x64
	@cp -r levels release/Windows-x64

	@cp -r assets release/Linux-x64
	@cp -r assets release/Windows-x64

	@cp LICENSE release/Linux-x64
	@cp LICENSE release/Windows-x64

	@tar -vczf release/Linux-x64.tar.gz release/Linux-x64/
	@zip -r release/Windows-x64.zip release/Windows-x64/

# Clean all builds
clear: 
	@rm -f $(OBJDIR)/*.o $(WINOBJDIR)/*.o wireframe wireframe.exe
