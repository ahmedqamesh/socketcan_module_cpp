# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/dcs/git/SocketCAN-cpp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/dcs/git/SocketCAN-cpp/build

# Include any dependencies generated for this target.
include CMakeFiles/CanWrapper.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/CanWrapper.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/CanWrapper.dir/flags.make

CanWrapper: CMakeFiles/CanWrapper.dir/build.make

.PHONY : CanWrapper

# Rule to build all files generated by this target.
CMakeFiles/CanWrapper.dir/build: CanWrapper

.PHONY : CMakeFiles/CanWrapper.dir/build

CMakeFiles/CanWrapper.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/CanWrapper.dir/cmake_clean.cmake
.PHONY : CMakeFiles/CanWrapper.dir/clean

CMakeFiles/CanWrapper.dir/depend:
	cd /home/dcs/git/SocketCAN-cpp/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dcs/git/SocketCAN-cpp /home/dcs/git/SocketCAN-cpp /home/dcs/git/SocketCAN-cpp/build /home/dcs/git/SocketCAN-cpp/build /home/dcs/git/SocketCAN-cpp/build/CMakeFiles/CanWrapper.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/CanWrapper.dir/depend
