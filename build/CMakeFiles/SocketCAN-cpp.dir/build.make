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
include CMakeFiles/SocketCAN-cpp.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/SocketCAN-cpp.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/SocketCAN-cpp.dir/flags.make

CMakeFiles/SocketCAN-cpp.dir/src/CanWrapper.cpp.o: CMakeFiles/SocketCAN-cpp.dir/flags.make
CMakeFiles/SocketCAN-cpp.dir/src/CanWrapper.cpp.o: ../src/CanWrapper.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dcs/git/SocketCAN-cpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/SocketCAN-cpp.dir/src/CanWrapper.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/SocketCAN-cpp.dir/src/CanWrapper.cpp.o -c /home/dcs/git/SocketCAN-cpp/src/CanWrapper.cpp

CMakeFiles/SocketCAN-cpp.dir/src/CanWrapper.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SocketCAN-cpp.dir/src/CanWrapper.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dcs/git/SocketCAN-cpp/src/CanWrapper.cpp > CMakeFiles/SocketCAN-cpp.dir/src/CanWrapper.cpp.i

CMakeFiles/SocketCAN-cpp.dir/src/CanWrapper.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SocketCAN-cpp.dir/src/CanWrapper.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dcs/git/SocketCAN-cpp/src/CanWrapper.cpp -o CMakeFiles/SocketCAN-cpp.dir/src/CanWrapper.cpp.s

SocketCAN-cpp: CMakeFiles/SocketCAN-cpp.dir/src/CanWrapper.cpp.o
SocketCAN-cpp: CMakeFiles/SocketCAN-cpp.dir/build.make

.PHONY : SocketCAN-cpp

# Rule to build all files generated by this target.
CMakeFiles/SocketCAN-cpp.dir/build: SocketCAN-cpp

.PHONY : CMakeFiles/SocketCAN-cpp.dir/build

CMakeFiles/SocketCAN-cpp.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/SocketCAN-cpp.dir/cmake_clean.cmake
.PHONY : CMakeFiles/SocketCAN-cpp.dir/clean

CMakeFiles/SocketCAN-cpp.dir/depend:
	cd /home/dcs/git/SocketCAN-cpp/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dcs/git/SocketCAN-cpp /home/dcs/git/SocketCAN-cpp /home/dcs/git/SocketCAN-cpp/build /home/dcs/git/SocketCAN-cpp/build /home/dcs/git/SocketCAN-cpp/build/CMakeFiles/SocketCAN-cpp.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/SocketCAN-cpp.dir/depend

