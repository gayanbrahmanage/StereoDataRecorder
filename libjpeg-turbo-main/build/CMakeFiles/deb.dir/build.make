# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_SOURCE_DIR = /home/bgs/CSI3DEDGE/libjpeg-turbo-main

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/bgs/CSI3DEDGE/libjpeg-turbo-main/build

# Utility rule file for deb.

# Include the progress variables for this target.
include CMakeFiles/deb.dir/progress.make

CMakeFiles/deb:
	pkgscripts/makedpkg

deb: CMakeFiles/deb
deb: CMakeFiles/deb.dir/build.make

.PHONY : deb

# Rule to build all files generated by this target.
CMakeFiles/deb.dir/build: deb

.PHONY : CMakeFiles/deb.dir/build

CMakeFiles/deb.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/deb.dir/cmake_clean.cmake
.PHONY : CMakeFiles/deb.dir/clean

CMakeFiles/deb.dir/depend:
	cd /home/bgs/CSI3DEDGE/libjpeg-turbo-main/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bgs/CSI3DEDGE/libjpeg-turbo-main /home/bgs/CSI3DEDGE/libjpeg-turbo-main /home/bgs/CSI3DEDGE/libjpeg-turbo-main/build /home/bgs/CSI3DEDGE/libjpeg-turbo-main/build /home/bgs/CSI3DEDGE/libjpeg-turbo-main/build/CMakeFiles/deb.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/deb.dir/depend

