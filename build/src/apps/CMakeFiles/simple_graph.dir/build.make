# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_SOURCE_DIR = /home/zetian/Workspace/sample_ltl/sampling_ltl

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/zetian/Workspace/sample_ltl/build

# Include any dependencies generated for this target.
include src/apps/CMakeFiles/simple_graph.dir/depend.make

# Include the progress variables for this target.
include src/apps/CMakeFiles/simple_graph.dir/progress.make

# Include the compile flags for this target's objects.
include src/apps/CMakeFiles/simple_graph.dir/flags.make

src/apps/CMakeFiles/simple_graph.dir/simple_graph.cpp.o: src/apps/CMakeFiles/simple_graph.dir/flags.make
src/apps/CMakeFiles/simple_graph.dir/simple_graph.cpp.o: /home/zetian/Workspace/sample_ltl/sampling_ltl/src/apps/simple_graph.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/zetian/Workspace/sample_ltl/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/apps/CMakeFiles/simple_graph.dir/simple_graph.cpp.o"
	cd /home/zetian/Workspace/sample_ltl/build/src/apps && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/simple_graph.dir/simple_graph.cpp.o -c /home/zetian/Workspace/sample_ltl/sampling_ltl/src/apps/simple_graph.cpp

src/apps/CMakeFiles/simple_graph.dir/simple_graph.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/simple_graph.dir/simple_graph.cpp.i"
	cd /home/zetian/Workspace/sample_ltl/build/src/apps && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/zetian/Workspace/sample_ltl/sampling_ltl/src/apps/simple_graph.cpp > CMakeFiles/simple_graph.dir/simple_graph.cpp.i

src/apps/CMakeFiles/simple_graph.dir/simple_graph.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/simple_graph.dir/simple_graph.cpp.s"
	cd /home/zetian/Workspace/sample_ltl/build/src/apps && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/zetian/Workspace/sample_ltl/sampling_ltl/src/apps/simple_graph.cpp -o CMakeFiles/simple_graph.dir/simple_graph.cpp.s

src/apps/CMakeFiles/simple_graph.dir/simple_graph.cpp.o.requires:
.PHONY : src/apps/CMakeFiles/simple_graph.dir/simple_graph.cpp.o.requires

src/apps/CMakeFiles/simple_graph.dir/simple_graph.cpp.o.provides: src/apps/CMakeFiles/simple_graph.dir/simple_graph.cpp.o.requires
	$(MAKE) -f src/apps/CMakeFiles/simple_graph.dir/build.make src/apps/CMakeFiles/simple_graph.dir/simple_graph.cpp.o.provides.build
.PHONY : src/apps/CMakeFiles/simple_graph.dir/simple_graph.cpp.o.provides

src/apps/CMakeFiles/simple_graph.dir/simple_graph.cpp.o.provides.build: src/apps/CMakeFiles/simple_graph.dir/simple_graph.cpp.o

# Object files for target simple_graph
simple_graph_OBJECTS = \
"CMakeFiles/simple_graph.dir/simple_graph.cpp.o"

# External object files for target simple_graph
simple_graph_EXTERNAL_OBJECTS =

bin/simple_graph: src/apps/CMakeFiles/simple_graph.dir/simple_graph.cpp.o
bin/simple_graph: src/apps/CMakeFiles/simple_graph.dir/build.make
bin/simple_graph: src/apps/CMakeFiles/simple_graph.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ../../bin/simple_graph"
	cd /home/zetian/Workspace/sample_ltl/build/src/apps && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/simple_graph.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/apps/CMakeFiles/simple_graph.dir/build: bin/simple_graph
.PHONY : src/apps/CMakeFiles/simple_graph.dir/build

src/apps/CMakeFiles/simple_graph.dir/requires: src/apps/CMakeFiles/simple_graph.dir/simple_graph.cpp.o.requires
.PHONY : src/apps/CMakeFiles/simple_graph.dir/requires

src/apps/CMakeFiles/simple_graph.dir/clean:
	cd /home/zetian/Workspace/sample_ltl/build/src/apps && $(CMAKE_COMMAND) -P CMakeFiles/simple_graph.dir/cmake_clean.cmake
.PHONY : src/apps/CMakeFiles/simple_graph.dir/clean

src/apps/CMakeFiles/simple_graph.dir/depend:
	cd /home/zetian/Workspace/sample_ltl/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zetian/Workspace/sample_ltl/sampling_ltl /home/zetian/Workspace/sample_ltl/sampling_ltl/src/apps /home/zetian/Workspace/sample_ltl/build /home/zetian/Workspace/sample_ltl/build/src/apps /home/zetian/Workspace/sample_ltl/build/src/apps/CMakeFiles/simple_graph.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/apps/CMakeFiles/simple_graph.dir/depend

