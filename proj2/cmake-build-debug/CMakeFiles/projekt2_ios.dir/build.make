# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

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
CMAKE_COMMAND = /home/stinky/cmake-3.20.6-linux-x86_64/bin/cmake

# The command to remove a file.
RM = /home/stinky/cmake-3.20.6-linux-x86_64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/d/ios/ios-proj2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/d/ios/ios-proj2/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/projekt2_ios.dir/depend.make
# Include the progress variables for this target.
include CMakeFiles/projekt2_ios.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/projekt2_ios.dir/flags.make

CMakeFiles/projekt2_ios.dir/proj2.c.o: CMakeFiles/projekt2_ios.dir/flags.make
CMakeFiles/projekt2_ios.dir/proj2.c.o: ../proj2.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/d/ios/ios-proj2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/projekt2_ios.dir/proj2.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/projekt2_ios.dir/proj2.c.o -c /mnt/d/ios/ios-proj2/proj2.c

CMakeFiles/projekt2_ios.dir/proj2.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/projekt2_ios.dir/proj2.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/d/ios/ios-proj2/proj2.c > CMakeFiles/projekt2_ios.dir/proj2.c.i

CMakeFiles/projekt2_ios.dir/proj2.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/projekt2_ios.dir/proj2.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/d/ios/ios-proj2/proj2.c -o CMakeFiles/projekt2_ios.dir/proj2.c.s

# Object files for target projekt2_ios
projekt2_ios_OBJECTS = \
"CMakeFiles/projekt2_ios.dir/proj2.c.o"

# External object files for target projekt2_ios
projekt2_ios_EXTERNAL_OBJECTS =

projekt2_ios: CMakeFiles/projekt2_ios.dir/proj2.c.o
projekt2_ios: CMakeFiles/projekt2_ios.dir/build.make
projekt2_ios: CMakeFiles/projekt2_ios.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/d/ios/ios-proj2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable projekt2_ios"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/projekt2_ios.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/projekt2_ios.dir/build: projekt2_ios
.PHONY : CMakeFiles/projekt2_ios.dir/build

CMakeFiles/projekt2_ios.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/projekt2_ios.dir/cmake_clean.cmake
.PHONY : CMakeFiles/projekt2_ios.dir/clean

CMakeFiles/projekt2_ios.dir/depend:
	cd /mnt/d/ios/ios-proj2/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/d/ios/ios-proj2 /mnt/d/ios/ios-proj2 /mnt/d/ios/ios-proj2/cmake-build-debug /mnt/d/ios/ios-proj2/cmake-build-debug /mnt/d/ios/ios-proj2/cmake-build-debug/CMakeFiles/projekt2_ios.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/projekt2_ios.dir/depend
