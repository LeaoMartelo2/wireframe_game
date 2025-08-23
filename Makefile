.PHONY: clear clean release

# compilers
CC 		:= clang++
WCC 		:= x86_64-w64-mingw32-g++

# layout
SRCDIR 		:= src
OBJDIR 		:= build/posix
WINOBJDIR 	:= build/win32
#BINDIR 		:= .

# sources

SRC 		:= $(wildcard $(SRCDIR)/*.cpp)
OBJ 		:= $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC))
WINOBJ 		:= $(patsubst $(SRCDIR)/%.cpp, $(WINOBJDIR)/%.o, $(SRC))
DEPS 		:= $(OBJ:.o=.d)
WINDEPS 	:= $(WINOBJ:.o=.d)


# flags
DISABLED_WARNINGS 	:= -Wno-missing-field-initializers -Wno-format-overflow -Wno-unused-command-line-argument -Wno-missing-braces -Wno-macro-redefined
CXX_FLAGS 		:= -std=c++20 -Wall -Wextra $(DISABLED_WARNINGS) -pedantic -lm 
CLANG_CONFIG 		:= -fdiagnostics-color=always -fdiagnostics-format=vi

ifdef DEBUG
	CXX_FLAGS += -DDEBUG -g
endif

RELEASE_FLAGS 		:= -O2 -DLOGNEST_DISABLE_DEBUG

# dependency
DEPFLAGS 		:= -MMD -MP

# linker flags
POSIX_LDFLAGS 		:= -L ./raylib/linux/ -lraylib
WINDOWS_LDFLAGS 	:= -L ./raylib/windows/ -lraylib -lgdi32 -lwinmm -lopengl32 -static -mwindows

all: wireframe


# binaries

wireframe: $(OBJ)
	$(CC) $^ $(CXX_FLAGS) $(POSIX_LDFLAGS) -o wireframe

win: wireframe.exe

wireframe.exe: $(WINOBJ)
	$(WCC) $^ $(CXX_FLAGS) $(WINDOWS_LDFLAGS) -o wireframe.exe


# posix compile rule
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp 
	@mkdir -p $(OBJDIR)

	@printf "\033[1m%s -> %s\033[0m\n" "$<" "$@"

	$(CC) $(CXX_FLAGS) $(DEPFLAGS) $(CLANG_CONFIG) -MF $(@:.o=.d) -c \
	$< -o $@ \


# win32 compile rule
$(WINOBJDIR)/%.o: $(SRCDIR)/%.cpp 
	@mkdir -p $(WINOBJDIR)

	@printf "\033[1m%s -> %s\033[0m\n" "$<" "$@"

	$(WCC) $(CXX_FLAGS) $(DEPFLAGS) -MF $(@:.o=.d) -c \
	$< -o $@ \

# include dep files if present, ignore if missing
-include $(DEPS)
-include $(WINDEPS)


# release

release: CXX_FLAGS += $(RELEASE_FLAGS)
release: wireframe win
	@mkdir -p release/Linux-x64 release/Windows-x64 
	@cp wireframe release/Linux-x64/ 			|| true
	@cp wireframe.exe release/Windows-x64/  		|| true
	@cp -r levels release/Linux-x64/  			|| true
	@cp -r levels release/Windows-x64/  			|| true
	@cp -r assets release/Linux-x64/  			|| true
	@cp -r assets release/Windows-x64/  			|| true
	@cp LICENSE release/Linux-x64/ release/Windows-x64/  	|| true
	@tar -vczf release/Linux-x64.tar.gz -C release Linux-x64
	@zip -r release/Windows-x64.zip release/Windows-x64/

clear:
	@rm -rf $(OBJDIR)/*.o $(WINOBJDIR)/*.o $(OBJDIR)/*.d $(WINOBJDIR)/*.d wireframe wireframe.exe release/

clean: clear

