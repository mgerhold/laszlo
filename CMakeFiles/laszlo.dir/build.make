# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/runner/work/laszlo/laszlo

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/runner/work/laszlo/laszlo/build

# Include any dependencies generated for this target.
include src/CMakeFiles/laszlo.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/CMakeFiles/laszlo.dir/compiler_depend.make

# Include the progress variables for this target.
include src/CMakeFiles/laszlo.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/laszlo.dir/flags.make

src/CMakeFiles/laszlo.dir/main.cpp.o: src/CMakeFiles/laszlo.dir/flags.make
src/CMakeFiles/laszlo.dir/main.cpp.o: /home/runner/work/laszlo/laszlo/src/main.cpp
src/CMakeFiles/laszlo.dir/main.cpp.o: src/CMakeFiles/laszlo.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/runner/work/laszlo/laszlo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/laszlo.dir/main.cpp.o"
	cd /home/runner/work/laszlo/laszlo/build/src && /home/runner/work/laszlo/laszlo/emsdk/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/laszlo.dir/main.cpp.o -MF CMakeFiles/laszlo.dir/main.cpp.o.d -o CMakeFiles/laszlo.dir/main.cpp.o -c /home/runner/work/laszlo/laszlo/src/main.cpp

src/CMakeFiles/laszlo.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/laszlo.dir/main.cpp.i"
	cd /home/runner/work/laszlo/laszlo/build/src && /home/runner/work/laszlo/laszlo/emsdk/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/runner/work/laszlo/laszlo/src/main.cpp > CMakeFiles/laszlo.dir/main.cpp.i

src/CMakeFiles/laszlo.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/laszlo.dir/main.cpp.s"
	cd /home/runner/work/laszlo/laszlo/build/src && /home/runner/work/laszlo/laszlo/emsdk/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/runner/work/laszlo/laszlo/src/main.cpp -o CMakeFiles/laszlo.dir/main.cpp.s

src/CMakeFiles/laszlo.dir/lexer.cpp.o: src/CMakeFiles/laszlo.dir/flags.make
src/CMakeFiles/laszlo.dir/lexer.cpp.o: /home/runner/work/laszlo/laszlo/src/lexer.cpp
src/CMakeFiles/laszlo.dir/lexer.cpp.o: src/CMakeFiles/laszlo.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/runner/work/laszlo/laszlo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/CMakeFiles/laszlo.dir/lexer.cpp.o"
	cd /home/runner/work/laszlo/laszlo/build/src && /home/runner/work/laszlo/laszlo/emsdk/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/laszlo.dir/lexer.cpp.o -MF CMakeFiles/laszlo.dir/lexer.cpp.o.d -o CMakeFiles/laszlo.dir/lexer.cpp.o -c /home/runner/work/laszlo/laszlo/src/lexer.cpp

src/CMakeFiles/laszlo.dir/lexer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/laszlo.dir/lexer.cpp.i"
	cd /home/runner/work/laszlo/laszlo/build/src && /home/runner/work/laszlo/laszlo/emsdk/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/runner/work/laszlo/laszlo/src/lexer.cpp > CMakeFiles/laszlo.dir/lexer.cpp.i

src/CMakeFiles/laszlo.dir/lexer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/laszlo.dir/lexer.cpp.s"
	cd /home/runner/work/laszlo/laszlo/build/src && /home/runner/work/laszlo/laszlo/emsdk/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/runner/work/laszlo/laszlo/src/lexer.cpp -o CMakeFiles/laszlo.dir/lexer.cpp.s

src/CMakeFiles/laszlo.dir/parser.cpp.o: src/CMakeFiles/laszlo.dir/flags.make
src/CMakeFiles/laszlo.dir/parser.cpp.o: /home/runner/work/laszlo/laszlo/src/parser.cpp
src/CMakeFiles/laszlo.dir/parser.cpp.o: src/CMakeFiles/laszlo.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/runner/work/laszlo/laszlo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/CMakeFiles/laszlo.dir/parser.cpp.o"
	cd /home/runner/work/laszlo/laszlo/build/src && /home/runner/work/laszlo/laszlo/emsdk/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/laszlo.dir/parser.cpp.o -MF CMakeFiles/laszlo.dir/parser.cpp.o.d -o CMakeFiles/laszlo.dir/parser.cpp.o -c /home/runner/work/laszlo/laszlo/src/parser.cpp

src/CMakeFiles/laszlo.dir/parser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/laszlo.dir/parser.cpp.i"
	cd /home/runner/work/laszlo/laszlo/build/src && /home/runner/work/laszlo/laszlo/emsdk/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/runner/work/laszlo/laszlo/src/parser.cpp > CMakeFiles/laszlo.dir/parser.cpp.i

src/CMakeFiles/laszlo.dir/parser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/laszlo.dir/parser.cpp.s"
	cd /home/runner/work/laszlo/laszlo/build/src && /home/runner/work/laszlo/laszlo/emsdk/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/runner/work/laszlo/laszlo/src/parser.cpp -o CMakeFiles/laszlo.dir/parser.cpp.s

src/CMakeFiles/laszlo.dir/values/integer.cpp.o: src/CMakeFiles/laszlo.dir/flags.make
src/CMakeFiles/laszlo.dir/values/integer.cpp.o: /home/runner/work/laszlo/laszlo/src/values/integer.cpp
src/CMakeFiles/laszlo.dir/values/integer.cpp.o: src/CMakeFiles/laszlo.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/runner/work/laszlo/laszlo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/CMakeFiles/laszlo.dir/values/integer.cpp.o"
	cd /home/runner/work/laszlo/laszlo/build/src && /home/runner/work/laszlo/laszlo/emsdk/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/laszlo.dir/values/integer.cpp.o -MF CMakeFiles/laszlo.dir/values/integer.cpp.o.d -o CMakeFiles/laszlo.dir/values/integer.cpp.o -c /home/runner/work/laszlo/laszlo/src/values/integer.cpp

src/CMakeFiles/laszlo.dir/values/integer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/laszlo.dir/values/integer.cpp.i"
	cd /home/runner/work/laszlo/laszlo/build/src && /home/runner/work/laszlo/laszlo/emsdk/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/runner/work/laszlo/laszlo/src/values/integer.cpp > CMakeFiles/laszlo.dir/values/integer.cpp.i

src/CMakeFiles/laszlo.dir/values/integer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/laszlo.dir/values/integer.cpp.s"
	cd /home/runner/work/laszlo/laszlo/build/src && /home/runner/work/laszlo/laszlo/emsdk/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/runner/work/laszlo/laszlo/src/values/integer.cpp -o CMakeFiles/laszlo.dir/values/integer.cpp.s

src/CMakeFiles/laszlo.dir/values/array.cpp.o: src/CMakeFiles/laszlo.dir/flags.make
src/CMakeFiles/laszlo.dir/values/array.cpp.o: /home/runner/work/laszlo/laszlo/src/values/array.cpp
src/CMakeFiles/laszlo.dir/values/array.cpp.o: src/CMakeFiles/laszlo.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/runner/work/laszlo/laszlo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object src/CMakeFiles/laszlo.dir/values/array.cpp.o"
	cd /home/runner/work/laszlo/laszlo/build/src && /home/runner/work/laszlo/laszlo/emsdk/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/laszlo.dir/values/array.cpp.o -MF CMakeFiles/laszlo.dir/values/array.cpp.o.d -o CMakeFiles/laszlo.dir/values/array.cpp.o -c /home/runner/work/laszlo/laszlo/src/values/array.cpp

src/CMakeFiles/laszlo.dir/values/array.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/laszlo.dir/values/array.cpp.i"
	cd /home/runner/work/laszlo/laszlo/build/src && /home/runner/work/laszlo/laszlo/emsdk/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/runner/work/laszlo/laszlo/src/values/array.cpp > CMakeFiles/laszlo.dir/values/array.cpp.i

src/CMakeFiles/laszlo.dir/values/array.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/laszlo.dir/values/array.cpp.s"
	cd /home/runner/work/laszlo/laszlo/build/src && /home/runner/work/laszlo/laszlo/emsdk/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/runner/work/laszlo/laszlo/src/values/array.cpp -o CMakeFiles/laszlo.dir/values/array.cpp.s

src/CMakeFiles/laszlo.dir/values/function.cpp.o: src/CMakeFiles/laszlo.dir/flags.make
src/CMakeFiles/laszlo.dir/values/function.cpp.o: /home/runner/work/laszlo/laszlo/src/values/function.cpp
src/CMakeFiles/laszlo.dir/values/function.cpp.o: src/CMakeFiles/laszlo.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/runner/work/laszlo/laszlo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object src/CMakeFiles/laszlo.dir/values/function.cpp.o"
	cd /home/runner/work/laszlo/laszlo/build/src && /home/runner/work/laszlo/laszlo/emsdk/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/laszlo.dir/values/function.cpp.o -MF CMakeFiles/laszlo.dir/values/function.cpp.o.d -o CMakeFiles/laszlo.dir/values/function.cpp.o -c /home/runner/work/laszlo/laszlo/src/values/function.cpp

src/CMakeFiles/laszlo.dir/values/function.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/laszlo.dir/values/function.cpp.i"
	cd /home/runner/work/laszlo/laszlo/build/src && /home/runner/work/laszlo/laszlo/emsdk/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/runner/work/laszlo/laszlo/src/values/function.cpp > CMakeFiles/laszlo.dir/values/function.cpp.i

src/CMakeFiles/laszlo.dir/values/function.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/laszlo.dir/values/function.cpp.s"
	cd /home/runner/work/laszlo/laszlo/build/src && /home/runner/work/laszlo/laszlo/emsdk/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/runner/work/laszlo/laszlo/src/values/function.cpp -o CMakeFiles/laszlo.dir/values/function.cpp.s

src/CMakeFiles/laszlo.dir/values/value.cpp.o: src/CMakeFiles/laszlo.dir/flags.make
src/CMakeFiles/laszlo.dir/values/value.cpp.o: /home/runner/work/laszlo/laszlo/src/values/value.cpp
src/CMakeFiles/laszlo.dir/values/value.cpp.o: src/CMakeFiles/laszlo.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/runner/work/laszlo/laszlo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object src/CMakeFiles/laszlo.dir/values/value.cpp.o"
	cd /home/runner/work/laszlo/laszlo/build/src && /home/runner/work/laszlo/laszlo/emsdk/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/laszlo.dir/values/value.cpp.o -MF CMakeFiles/laszlo.dir/values/value.cpp.o.d -o CMakeFiles/laszlo.dir/values/value.cpp.o -c /home/runner/work/laszlo/laszlo/src/values/value.cpp

src/CMakeFiles/laszlo.dir/values/value.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/laszlo.dir/values/value.cpp.i"
	cd /home/runner/work/laszlo/laszlo/build/src && /home/runner/work/laszlo/laszlo/emsdk/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/runner/work/laszlo/laszlo/src/values/value.cpp > CMakeFiles/laszlo.dir/values/value.cpp.i

src/CMakeFiles/laszlo.dir/values/value.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/laszlo.dir/values/value.cpp.s"
	cd /home/runner/work/laszlo/laszlo/build/src && /home/runner/work/laszlo/laszlo/emsdk/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/runner/work/laszlo/laszlo/src/values/value.cpp -o CMakeFiles/laszlo.dir/values/value.cpp.s

src/CMakeFiles/laszlo.dir/values/string.cpp.o: src/CMakeFiles/laszlo.dir/flags.make
src/CMakeFiles/laszlo.dir/values/string.cpp.o: /home/runner/work/laszlo/laszlo/src/values/string.cpp
src/CMakeFiles/laszlo.dir/values/string.cpp.o: src/CMakeFiles/laszlo.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/runner/work/laszlo/laszlo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object src/CMakeFiles/laszlo.dir/values/string.cpp.o"
	cd /home/runner/work/laszlo/laszlo/build/src && /home/runner/work/laszlo/laszlo/emsdk/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/laszlo.dir/values/string.cpp.o -MF CMakeFiles/laszlo.dir/values/string.cpp.o.d -o CMakeFiles/laszlo.dir/values/string.cpp.o -c /home/runner/work/laszlo/laszlo/src/values/string.cpp

src/CMakeFiles/laszlo.dir/values/string.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/laszlo.dir/values/string.cpp.i"
	cd /home/runner/work/laszlo/laszlo/build/src && /home/runner/work/laszlo/laszlo/emsdk/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/runner/work/laszlo/laszlo/src/values/string.cpp > CMakeFiles/laszlo.dir/values/string.cpp.i

src/CMakeFiles/laszlo.dir/values/string.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/laszlo.dir/values/string.cpp.s"
	cd /home/runner/work/laszlo/laszlo/build/src && /home/runner/work/laszlo/laszlo/emsdk/upstream/emscripten/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/runner/work/laszlo/laszlo/src/values/string.cpp -o CMakeFiles/laszlo.dir/values/string.cpp.s

# Object files for target laszlo
laszlo_OBJECTS = \
"CMakeFiles/laszlo.dir/main.cpp.o" \
"CMakeFiles/laszlo.dir/lexer.cpp.o" \
"CMakeFiles/laszlo.dir/parser.cpp.o" \
"CMakeFiles/laszlo.dir/values/integer.cpp.o" \
"CMakeFiles/laszlo.dir/values/array.cpp.o" \
"CMakeFiles/laszlo.dir/values/function.cpp.o" \
"CMakeFiles/laszlo.dir/values/value.cpp.o" \
"CMakeFiles/laszlo.dir/values/string.cpp.o"

# External object files for target laszlo
laszlo_EXTERNAL_OBJECTS =

src/laszlo.html: src/CMakeFiles/laszlo.dir/main.cpp.o
src/laszlo.html: src/CMakeFiles/laszlo.dir/lexer.cpp.o
src/laszlo.html: src/CMakeFiles/laszlo.dir/parser.cpp.o
src/laszlo.html: src/CMakeFiles/laszlo.dir/values/integer.cpp.o
src/laszlo.html: src/CMakeFiles/laszlo.dir/values/array.cpp.o
src/laszlo.html: src/CMakeFiles/laszlo.dir/values/function.cpp.o
src/laszlo.html: src/CMakeFiles/laszlo.dir/values/value.cpp.o
src/laszlo.html: src/CMakeFiles/laszlo.dir/values/string.cpp.o
src/laszlo.html: src/CMakeFiles/laszlo.dir/build.make
src/laszlo.html: src/CMakeFiles/laszlo.dir/objects1.rsp
src/laszlo.html: src/CMakeFiles/laszlo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/runner/work/laszlo/laszlo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Linking CXX executable laszlo.html"
	cd /home/runner/work/laszlo/laszlo/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/laszlo.dir/link.txt --verbose=$(VERBOSE)
	cd /home/runner/work/laszlo/laszlo/build/src && /usr/local/bin/cmake -E copy /home/runner/work/laszlo/laszlo/emscripten/mode-laszlo.js /home/runner/work/laszlo/laszlo/build/src/mode-laszlo.js

# Rule to build all files generated by this target.
src/CMakeFiles/laszlo.dir/build: src/laszlo.html
.PHONY : src/CMakeFiles/laszlo.dir/build

src/CMakeFiles/laszlo.dir/clean:
	cd /home/runner/work/laszlo/laszlo/build/src && $(CMAKE_COMMAND) -P CMakeFiles/laszlo.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/laszlo.dir/clean

src/CMakeFiles/laszlo.dir/depend:
	cd /home/runner/work/laszlo/laszlo/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/runner/work/laszlo/laszlo /home/runner/work/laszlo/laszlo/src /home/runner/work/laszlo/laszlo/build /home/runner/work/laszlo/laszlo/build/src /home/runner/work/laszlo/laszlo/build/src/CMakeFiles/laszlo.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : src/CMakeFiles/laszlo.dir/depend

