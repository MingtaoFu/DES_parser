# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.7

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/mingtao/CLionProjects/DES_parser

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/mingtao/CLionProjects/DES_parser/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/DES_parser.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/DES_parser.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/DES_parser.dir/flags.make

CMakeFiles/DES_parser.dir/main.c.o: CMakeFiles/DES_parser.dir/flags.make
CMakeFiles/DES_parser.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mingtao/CLionProjects/DES_parser/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/DES_parser.dir/main.c.o"
	/usr/lib/hardening-wrapper/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/DES_parser.dir/main.c.o   -c /home/mingtao/CLionProjects/DES_parser/main.c

CMakeFiles/DES_parser.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/DES_parser.dir/main.c.i"
	/usr/lib/hardening-wrapper/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/mingtao/CLionProjects/DES_parser/main.c > CMakeFiles/DES_parser.dir/main.c.i

CMakeFiles/DES_parser.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/DES_parser.dir/main.c.s"
	/usr/lib/hardening-wrapper/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/mingtao/CLionProjects/DES_parser/main.c -o CMakeFiles/DES_parser.dir/main.c.s

CMakeFiles/DES_parser.dir/main.c.o.requires:

.PHONY : CMakeFiles/DES_parser.dir/main.c.o.requires

CMakeFiles/DES_parser.dir/main.c.o.provides: CMakeFiles/DES_parser.dir/main.c.o.requires
	$(MAKE) -f CMakeFiles/DES_parser.dir/build.make CMakeFiles/DES_parser.dir/main.c.o.provides.build
.PHONY : CMakeFiles/DES_parser.dir/main.c.o.provides

CMakeFiles/DES_parser.dir/main.c.o.provides.build: CMakeFiles/DES_parser.dir/main.c.o


# Object files for target DES_parser
DES_parser_OBJECTS = \
"CMakeFiles/DES_parser.dir/main.c.o"

# External object files for target DES_parser
DES_parser_EXTERNAL_OBJECTS =

DES_parser: CMakeFiles/DES_parser.dir/main.c.o
DES_parser: CMakeFiles/DES_parser.dir/build.make
DES_parser: CMakeFiles/DES_parser.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/mingtao/CLionProjects/DES_parser/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable DES_parser"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/DES_parser.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/DES_parser.dir/build: DES_parser

.PHONY : CMakeFiles/DES_parser.dir/build

CMakeFiles/DES_parser.dir/requires: CMakeFiles/DES_parser.dir/main.c.o.requires

.PHONY : CMakeFiles/DES_parser.dir/requires

CMakeFiles/DES_parser.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/DES_parser.dir/cmake_clean.cmake
.PHONY : CMakeFiles/DES_parser.dir/clean

CMakeFiles/DES_parser.dir/depend:
	cd /home/mingtao/CLionProjects/DES_parser/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mingtao/CLionProjects/DES_parser /home/mingtao/CLionProjects/DES_parser /home/mingtao/CLionProjects/DES_parser/cmake-build-debug /home/mingtao/CLionProjects/DES_parser/cmake-build-debug /home/mingtao/CLionProjects/DES_parser/cmake-build-debug/CMakeFiles/DES_parser.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/DES_parser.dir/depend

