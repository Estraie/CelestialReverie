# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.31

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = D:\CMake\bin\cmake.exe

# The command to remove a file.
RM = D:\CMake\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\Code\GlaxyEngine\CelestialReverie\test1\CelestialReverie

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\Code\GlaxyEngine\CelestialReverie\test1\CelestialReverie\build

# Include any dependencies generated for this target.
include CMakeFiles/TestPhys.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/TestPhys.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/TestPhys.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/TestPhys.dir/flags.make

CMakeFiles/TestPhys.dir/codegen:
.PHONY : CMakeFiles/TestPhys.dir/codegen

CMakeFiles/TestPhys.dir/main.cpp.obj: CMakeFiles/TestPhys.dir/flags.make
CMakeFiles/TestPhys.dir/main.cpp.obj: CMakeFiles/TestPhys.dir/includes_CXX.rsp
CMakeFiles/TestPhys.dir/main.cpp.obj: D:/Code/GlaxyEngine/CelestialReverie/test1/CelestialReverie/main.cpp
CMakeFiles/TestPhys.dir/main.cpp.obj: CMakeFiles/TestPhys.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=D:\Code\GlaxyEngine\CelestialReverie\test1\CelestialReverie\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/TestPhys.dir/main.cpp.obj"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TestPhys.dir/main.cpp.obj -MF CMakeFiles\TestPhys.dir\main.cpp.obj.d -o CMakeFiles\TestPhys.dir\main.cpp.obj -c D:\Code\GlaxyEngine\CelestialReverie\test1\CelestialReverie\main.cpp

CMakeFiles/TestPhys.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/TestPhys.dir/main.cpp.i"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Code\GlaxyEngine\CelestialReverie\test1\CelestialReverie\main.cpp > CMakeFiles\TestPhys.dir\main.cpp.i

CMakeFiles/TestPhys.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/TestPhys.dir/main.cpp.s"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Code\GlaxyEngine\CelestialReverie\test1\CelestialReverie\main.cpp -o CMakeFiles\TestPhys.dir\main.cpp.s

# Object files for target TestPhys
TestPhys_OBJECTS = \
"CMakeFiles/TestPhys.dir/main.cpp.obj"

# External object files for target TestPhys
TestPhys_EXTERNAL_OBJECTS =

TestPhys.exe: CMakeFiles/TestPhys.dir/main.cpp.obj
TestPhys.exe: CMakeFiles/TestPhys.dir/build.make
TestPhys.exe: CMakeFiles/TestPhys.dir/linkLibs.rsp
TestPhys.exe: CMakeFiles/TestPhys.dir/objects1.rsp
TestPhys.exe: CMakeFiles/TestPhys.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=D:\Code\GlaxyEngine\CelestialReverie\test1\CelestialReverie\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable TestPhys.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\TestPhys.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/TestPhys.dir/build: TestPhys.exe
.PHONY : CMakeFiles/TestPhys.dir/build

CMakeFiles/TestPhys.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\TestPhys.dir\cmake_clean.cmake
.PHONY : CMakeFiles/TestPhys.dir/clean

CMakeFiles/TestPhys.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\Code\GlaxyEngine\CelestialReverie\test1\CelestialReverie D:\Code\GlaxyEngine\CelestialReverie\test1\CelestialReverie D:\Code\GlaxyEngine\CelestialReverie\test1\CelestialReverie\build D:\Code\GlaxyEngine\CelestialReverie\test1\CelestialReverie\build D:\Code\GlaxyEngine\CelestialReverie\test1\CelestialReverie\build\CMakeFiles\TestPhys.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/TestPhys.dir/depend

