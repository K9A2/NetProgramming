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
CMAKE_COMMAND = /home/stormlin/program/clion/bin/cmake/bin/cmake

# The command to remove a file.
RM = /home/stormlin/program/clion/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/stormlin/project/NetProgramming/select/select

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/stormlin/project/NetProgramming/select/select/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/select.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/select.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/select.dir/flags.make

CMakeFiles/select.dir/select_cli.c.o: CMakeFiles/select.dir/flags.make
CMakeFiles/select.dir/select_cli.c.o: ../select_cli.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/stormlin/project/NetProgramming/select/select/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/select.dir/select_cli.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/select.dir/select_cli.c.o   -c /home/stormlin/project/NetProgramming/select/select/select_cli.c

CMakeFiles/select.dir/select_cli.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/select.dir/select_cli.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/stormlin/project/NetProgramming/select/select/select_cli.c > CMakeFiles/select.dir/select_cli.c.i

CMakeFiles/select.dir/select_cli.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/select.dir/select_cli.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/stormlin/project/NetProgramming/select/select/select_cli.c -o CMakeFiles/select.dir/select_cli.c.s

CMakeFiles/select.dir/select_cli.c.o.requires:

.PHONY : CMakeFiles/select.dir/select_cli.c.o.requires

CMakeFiles/select.dir/select_cli.c.o.provides: CMakeFiles/select.dir/select_cli.c.o.requires
	$(MAKE) -f CMakeFiles/select.dir/build.make CMakeFiles/select.dir/select_cli.c.o.provides.build
.PHONY : CMakeFiles/select.dir/select_cli.c.o.provides

CMakeFiles/select.dir/select_cli.c.o.provides.build: CMakeFiles/select.dir/select_cli.c.o


CMakeFiles/select.dir/select_server.c.o: CMakeFiles/select.dir/flags.make
CMakeFiles/select.dir/select_server.c.o: ../select_server.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/stormlin/project/NetProgramming/select/select/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/select.dir/select_server.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/select.dir/select_server.c.o   -c /home/stormlin/project/NetProgramming/select/select/select_server.c

CMakeFiles/select.dir/select_server.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/select.dir/select_server.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/stormlin/project/NetProgramming/select/select/select_server.c > CMakeFiles/select.dir/select_server.c.i

CMakeFiles/select.dir/select_server.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/select.dir/select_server.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/stormlin/project/NetProgramming/select/select/select_server.c -o CMakeFiles/select.dir/select_server.c.s

CMakeFiles/select.dir/select_server.c.o.requires:

.PHONY : CMakeFiles/select.dir/select_server.c.o.requires

CMakeFiles/select.dir/select_server.c.o.provides: CMakeFiles/select.dir/select_server.c.o.requires
	$(MAKE) -f CMakeFiles/select.dir/build.make CMakeFiles/select.dir/select_server.c.o.provides.build
.PHONY : CMakeFiles/select.dir/select_server.c.o.provides

CMakeFiles/select.dir/select_server.c.o.provides.build: CMakeFiles/select.dir/select_server.c.o


CMakeFiles/select.dir/select_server_good.c.o: CMakeFiles/select.dir/flags.make
CMakeFiles/select.dir/select_server_good.c.o: ../select_server_good.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/stormlin/project/NetProgramming/select/select/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/select.dir/select_server_good.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/select.dir/select_server_good.c.o   -c /home/stormlin/project/NetProgramming/select/select/select_server_good.c

CMakeFiles/select.dir/select_server_good.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/select.dir/select_server_good.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/stormlin/project/NetProgramming/select/select/select_server_good.c > CMakeFiles/select.dir/select_server_good.c.i

CMakeFiles/select.dir/select_server_good.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/select.dir/select_server_good.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/stormlin/project/NetProgramming/select/select/select_server_good.c -o CMakeFiles/select.dir/select_server_good.c.s

CMakeFiles/select.dir/select_server_good.c.o.requires:

.PHONY : CMakeFiles/select.dir/select_server_good.c.o.requires

CMakeFiles/select.dir/select_server_good.c.o.provides: CMakeFiles/select.dir/select_server_good.c.o.requires
	$(MAKE) -f CMakeFiles/select.dir/build.make CMakeFiles/select.dir/select_server_good.c.o.provides.build
.PHONY : CMakeFiles/select.dir/select_server_good.c.o.provides

CMakeFiles/select.dir/select_server_good.c.o.provides.build: CMakeFiles/select.dir/select_server_good.c.o


# Object files for target select
select_OBJECTS = \
"CMakeFiles/select.dir/select_cli.c.o" \
"CMakeFiles/select.dir/select_server.c.o" \
"CMakeFiles/select.dir/select_server_good.c.o"

# External object files for target select
select_EXTERNAL_OBJECTS =

select: CMakeFiles/select.dir/select_cli.c.o
select: CMakeFiles/select.dir/select_server.c.o
select: CMakeFiles/select.dir/select_server_good.c.o
select: CMakeFiles/select.dir/build.make
select: CMakeFiles/select.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/stormlin/project/NetProgramming/select/select/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable select"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/select.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/select.dir/build: select

.PHONY : CMakeFiles/select.dir/build

CMakeFiles/select.dir/requires: CMakeFiles/select.dir/select_cli.c.o.requires
CMakeFiles/select.dir/requires: CMakeFiles/select.dir/select_server.c.o.requires
CMakeFiles/select.dir/requires: CMakeFiles/select.dir/select_server_good.c.o.requires

.PHONY : CMakeFiles/select.dir/requires

CMakeFiles/select.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/select.dir/cmake_clean.cmake
.PHONY : CMakeFiles/select.dir/clean

CMakeFiles/select.dir/depend:
	cd /home/stormlin/project/NetProgramming/select/select/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/stormlin/project/NetProgramming/select/select /home/stormlin/project/NetProgramming/select/select /home/stormlin/project/NetProgramming/select/select/cmake-build-debug /home/stormlin/project/NetProgramming/select/select/cmake-build-debug /home/stormlin/project/NetProgramming/select/select/cmake-build-debug/CMakeFiles/select.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/select.dir/depend

