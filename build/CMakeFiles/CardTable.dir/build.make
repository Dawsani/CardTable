# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/dawson/Desktop/projects/card_table

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/dawson/Desktop/projects/card_table/build

# Include any dependencies generated for this target.
include CMakeFiles/CardTable.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/CardTable.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/CardTable.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/CardTable.dir/flags.make

CMakeFiles/CardTable.dir/src/main.cpp.o: CMakeFiles/CardTable.dir/flags.make
CMakeFiles/CardTable.dir/src/main.cpp.o: /home/dawson/Desktop/projects/card_table/src/main.cpp
CMakeFiles/CardTable.dir/src/main.cpp.o: CMakeFiles/CardTable.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/dawson/Desktop/projects/card_table/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/CardTable.dir/src/main.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CardTable.dir/src/main.cpp.o -MF CMakeFiles/CardTable.dir/src/main.cpp.o.d -o CMakeFiles/CardTable.dir/src/main.cpp.o -c /home/dawson/Desktop/projects/card_table/src/main.cpp

CMakeFiles/CardTable.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/CardTable.dir/src/main.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dawson/Desktop/projects/card_table/src/main.cpp > CMakeFiles/CardTable.dir/src/main.cpp.i

CMakeFiles/CardTable.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/CardTable.dir/src/main.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dawson/Desktop/projects/card_table/src/main.cpp -o CMakeFiles/CardTable.dir/src/main.cpp.s

CMakeFiles/CardTable.dir/src/Engine.cpp.o: CMakeFiles/CardTable.dir/flags.make
CMakeFiles/CardTable.dir/src/Engine.cpp.o: /home/dawson/Desktop/projects/card_table/src/Engine.cpp
CMakeFiles/CardTable.dir/src/Engine.cpp.o: CMakeFiles/CardTable.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/dawson/Desktop/projects/card_table/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/CardTable.dir/src/Engine.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CardTable.dir/src/Engine.cpp.o -MF CMakeFiles/CardTable.dir/src/Engine.cpp.o.d -o CMakeFiles/CardTable.dir/src/Engine.cpp.o -c /home/dawson/Desktop/projects/card_table/src/Engine.cpp

CMakeFiles/CardTable.dir/src/Engine.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/CardTable.dir/src/Engine.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dawson/Desktop/projects/card_table/src/Engine.cpp > CMakeFiles/CardTable.dir/src/Engine.cpp.i

CMakeFiles/CardTable.dir/src/Engine.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/CardTable.dir/src/Engine.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dawson/Desktop/projects/card_table/src/Engine.cpp -o CMakeFiles/CardTable.dir/src/Engine.cpp.s

# Object files for target CardTable
CardTable_OBJECTS = \
"CMakeFiles/CardTable.dir/src/main.cpp.o" \
"CMakeFiles/CardTable.dir/src/Engine.cpp.o"

# External object files for target CardTable
CardTable_EXTERNAL_OBJECTS =

CardTable: CMakeFiles/CardTable.dir/src/main.cpp.o
CardTable: CMakeFiles/CardTable.dir/src/Engine.cpp.o
CardTable: CMakeFiles/CardTable.dir/build.make
CardTable: CMakeFiles/CardTable.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/dawson/Desktop/projects/card_table/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable CardTable"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/CardTable.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/CardTable.dir/build: CardTable
.PHONY : CMakeFiles/CardTable.dir/build

CMakeFiles/CardTable.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/CardTable.dir/cmake_clean.cmake
.PHONY : CMakeFiles/CardTable.dir/clean

CMakeFiles/CardTable.dir/depend:
	cd /home/dawson/Desktop/projects/card_table/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dawson/Desktop/projects/card_table /home/dawson/Desktop/projects/card_table /home/dawson/Desktop/projects/card_table/build /home/dawson/Desktop/projects/card_table/build /home/dawson/Desktop/projects/card_table/build/CMakeFiles/CardTable.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/CardTable.dir/depend

