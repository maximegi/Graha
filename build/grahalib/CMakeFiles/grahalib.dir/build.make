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
CMAKE_SOURCE_DIR = /home/theo/Bureau/Projet/Graha

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/theo/Bureau/Projet/Graha/build

# Include any dependencies generated for this target.
include grahalib/CMakeFiles/grahalib.dir/depend.make

# Include the progress variables for this target.
include grahalib/CMakeFiles/grahalib.dir/progress.make

# Include the compile flags for this target's objects.
include grahalib/CMakeFiles/grahalib.dir/flags.make

grahalib/CMakeFiles/grahalib.dir/src/Game.cpp.o: grahalib/CMakeFiles/grahalib.dir/flags.make
grahalib/CMakeFiles/grahalib.dir/src/Game.cpp.o: ../grahalib/src/Game.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/theo/Bureau/Projet/Graha/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object grahalib/CMakeFiles/grahalib.dir/src/Game.cpp.o"
	cd /home/theo/Bureau/Projet/Graha/build/grahalib && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/grahalib.dir/src/Game.cpp.o -c /home/theo/Bureau/Projet/Graha/grahalib/src/Game.cpp

grahalib/CMakeFiles/grahalib.dir/src/Game.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/grahalib.dir/src/Game.cpp.i"
	cd /home/theo/Bureau/Projet/Graha/build/grahalib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/theo/Bureau/Projet/Graha/grahalib/src/Game.cpp > CMakeFiles/grahalib.dir/src/Game.cpp.i

grahalib/CMakeFiles/grahalib.dir/src/Game.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/grahalib.dir/src/Game.cpp.s"
	cd /home/theo/Bureau/Projet/Graha/build/grahalib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/theo/Bureau/Projet/Graha/grahalib/src/Game.cpp -o CMakeFiles/grahalib.dir/src/Game.cpp.s

grahalib/CMakeFiles/grahalib.dir/src/Game.cpp.o.requires:

.PHONY : grahalib/CMakeFiles/grahalib.dir/src/Game.cpp.o.requires

grahalib/CMakeFiles/grahalib.dir/src/Game.cpp.o.provides: grahalib/CMakeFiles/grahalib.dir/src/Game.cpp.o.requires
	$(MAKE) -f grahalib/CMakeFiles/grahalib.dir/build.make grahalib/CMakeFiles/grahalib.dir/src/Game.cpp.o.provides.build
.PHONY : grahalib/CMakeFiles/grahalib.dir/src/Game.cpp.o.provides

grahalib/CMakeFiles/grahalib.dir/src/Game.cpp.o.provides.build: grahalib/CMakeFiles/grahalib.dir/src/Game.cpp.o


grahalib/CMakeFiles/grahalib.dir/src/Planet.cpp.o: grahalib/CMakeFiles/grahalib.dir/flags.make
grahalib/CMakeFiles/grahalib.dir/src/Planet.cpp.o: ../grahalib/src/Planet.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/theo/Bureau/Projet/Graha/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object grahalib/CMakeFiles/grahalib.dir/src/Planet.cpp.o"
	cd /home/theo/Bureau/Projet/Graha/build/grahalib && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/grahalib.dir/src/Planet.cpp.o -c /home/theo/Bureau/Projet/Graha/grahalib/src/Planet.cpp

grahalib/CMakeFiles/grahalib.dir/src/Planet.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/grahalib.dir/src/Planet.cpp.i"
	cd /home/theo/Bureau/Projet/Graha/build/grahalib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/theo/Bureau/Projet/Graha/grahalib/src/Planet.cpp > CMakeFiles/grahalib.dir/src/Planet.cpp.i

grahalib/CMakeFiles/grahalib.dir/src/Planet.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/grahalib.dir/src/Planet.cpp.s"
	cd /home/theo/Bureau/Projet/Graha/build/grahalib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/theo/Bureau/Projet/Graha/grahalib/src/Planet.cpp -o CMakeFiles/grahalib.dir/src/Planet.cpp.s

grahalib/CMakeFiles/grahalib.dir/src/Planet.cpp.o.requires:

.PHONY : grahalib/CMakeFiles/grahalib.dir/src/Planet.cpp.o.requires

grahalib/CMakeFiles/grahalib.dir/src/Planet.cpp.o.provides: grahalib/CMakeFiles/grahalib.dir/src/Planet.cpp.o.requires
	$(MAKE) -f grahalib/CMakeFiles/grahalib.dir/build.make grahalib/CMakeFiles/grahalib.dir/src/Planet.cpp.o.provides.build
.PHONY : grahalib/CMakeFiles/grahalib.dir/src/Planet.cpp.o.provides

grahalib/CMakeFiles/grahalib.dir/src/Planet.cpp.o.provides.build: grahalib/CMakeFiles/grahalib.dir/src/Planet.cpp.o


grahalib/CMakeFiles/grahalib.dir/src/Text.cpp.o: grahalib/CMakeFiles/grahalib.dir/flags.make
grahalib/CMakeFiles/grahalib.dir/src/Text.cpp.o: ../grahalib/src/Text.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/theo/Bureau/Projet/Graha/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object grahalib/CMakeFiles/grahalib.dir/src/Text.cpp.o"
	cd /home/theo/Bureau/Projet/Graha/build/grahalib && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/grahalib.dir/src/Text.cpp.o -c /home/theo/Bureau/Projet/Graha/grahalib/src/Text.cpp

grahalib/CMakeFiles/grahalib.dir/src/Text.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/grahalib.dir/src/Text.cpp.i"
	cd /home/theo/Bureau/Projet/Graha/build/grahalib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/theo/Bureau/Projet/Graha/grahalib/src/Text.cpp > CMakeFiles/grahalib.dir/src/Text.cpp.i

grahalib/CMakeFiles/grahalib.dir/src/Text.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/grahalib.dir/src/Text.cpp.s"
	cd /home/theo/Bureau/Projet/Graha/build/grahalib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/theo/Bureau/Projet/Graha/grahalib/src/Text.cpp -o CMakeFiles/grahalib.dir/src/Text.cpp.s

grahalib/CMakeFiles/grahalib.dir/src/Text.cpp.o.requires:

.PHONY : grahalib/CMakeFiles/grahalib.dir/src/Text.cpp.o.requires

grahalib/CMakeFiles/grahalib.dir/src/Text.cpp.o.provides: grahalib/CMakeFiles/grahalib.dir/src/Text.cpp.o.requires
	$(MAKE) -f grahalib/CMakeFiles/grahalib.dir/build.make grahalib/CMakeFiles/grahalib.dir/src/Text.cpp.o.provides.build
.PHONY : grahalib/CMakeFiles/grahalib.dir/src/Text.cpp.o.provides

grahalib/CMakeFiles/grahalib.dir/src/Text.cpp.o.provides.build: grahalib/CMakeFiles/grahalib.dir/src/Text.cpp.o


# Object files for target grahalib
grahalib_OBJECTS = \
"CMakeFiles/grahalib.dir/src/Game.cpp.o" \
"CMakeFiles/grahalib.dir/src/Planet.cpp.o" \
"CMakeFiles/grahalib.dir/src/Text.cpp.o"

# External object files for target grahalib
grahalib_EXTERNAL_OBJECTS =

grahalib/libgrahalib.a: grahalib/CMakeFiles/grahalib.dir/src/Game.cpp.o
grahalib/libgrahalib.a: grahalib/CMakeFiles/grahalib.dir/src/Planet.cpp.o
grahalib/libgrahalib.a: grahalib/CMakeFiles/grahalib.dir/src/Text.cpp.o
grahalib/libgrahalib.a: grahalib/CMakeFiles/grahalib.dir/build.make
grahalib/libgrahalib.a: grahalib/CMakeFiles/grahalib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/theo/Bureau/Projet/Graha/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX static library libgrahalib.a"
	cd /home/theo/Bureau/Projet/Graha/build/grahalib && $(CMAKE_COMMAND) -P CMakeFiles/grahalib.dir/cmake_clean_target.cmake
	cd /home/theo/Bureau/Projet/Graha/build/grahalib && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/grahalib.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
grahalib/CMakeFiles/grahalib.dir/build: grahalib/libgrahalib.a

.PHONY : grahalib/CMakeFiles/grahalib.dir/build

grahalib/CMakeFiles/grahalib.dir/requires: grahalib/CMakeFiles/grahalib.dir/src/Game.cpp.o.requires
grahalib/CMakeFiles/grahalib.dir/requires: grahalib/CMakeFiles/grahalib.dir/src/Planet.cpp.o.requires
grahalib/CMakeFiles/grahalib.dir/requires: grahalib/CMakeFiles/grahalib.dir/src/Text.cpp.o.requires

.PHONY : grahalib/CMakeFiles/grahalib.dir/requires

grahalib/CMakeFiles/grahalib.dir/clean:
	cd /home/theo/Bureau/Projet/Graha/build/grahalib && $(CMAKE_COMMAND) -P CMakeFiles/grahalib.dir/cmake_clean.cmake
.PHONY : grahalib/CMakeFiles/grahalib.dir/clean

grahalib/CMakeFiles/grahalib.dir/depend:
	cd /home/theo/Bureau/Projet/Graha/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/theo/Bureau/Projet/Graha /home/theo/Bureau/Projet/Graha/grahalib /home/theo/Bureau/Projet/Graha/build /home/theo/Bureau/Projet/Graha/build/grahalib /home/theo/Bureau/Projet/Graha/build/grahalib/CMakeFiles/grahalib.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : grahalib/CMakeFiles/grahalib.dir/depend

