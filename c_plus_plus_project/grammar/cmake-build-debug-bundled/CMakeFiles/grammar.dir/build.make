# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.9

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
CMAKE_COMMAND = /root/mydev/tools/clion/bin/cmake/bin/cmake

# The command to remove a file.
RM = /root/mydev/tools/clion/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /root/mydev/myfiles/library/grammar

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/mydev/myfiles/library/grammar/cmake-build-debug-bundled

# Include any dependencies generated for this target.
include CMakeFiles/grammar.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/grammar.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/grammar.dir/flags.make

CMakeFiles/grammar.dir/main.cpp.o: CMakeFiles/grammar.dir/flags.make
CMakeFiles/grammar.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/mydev/myfiles/library/grammar/cmake-build-debug-bundled/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/grammar.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/grammar.dir/main.cpp.o -c /root/mydev/myfiles/library/grammar/main.cpp

CMakeFiles/grammar.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/grammar.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/mydev/myfiles/library/grammar/main.cpp > CMakeFiles/grammar.dir/main.cpp.i

CMakeFiles/grammar.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/grammar.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/mydev/myfiles/library/grammar/main.cpp -o CMakeFiles/grammar.dir/main.cpp.s

CMakeFiles/grammar.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/grammar.dir/main.cpp.o.requires

CMakeFiles/grammar.dir/main.cpp.o.provides: CMakeFiles/grammar.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/grammar.dir/build.make CMakeFiles/grammar.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/grammar.dir/main.cpp.o.provides

CMakeFiles/grammar.dir/main.cpp.o.provides.build: CMakeFiles/grammar.dir/main.cpp.o


CMakeFiles/grammar.dir/wd_grammar.cpp.o: CMakeFiles/grammar.dir/flags.make
CMakeFiles/grammar.dir/wd_grammar.cpp.o: ../wd_grammar.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/mydev/myfiles/library/grammar/cmake-build-debug-bundled/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/grammar.dir/wd_grammar.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/grammar.dir/wd_grammar.cpp.o -c /root/mydev/myfiles/library/grammar/wd_grammar.cpp

CMakeFiles/grammar.dir/wd_grammar.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/grammar.dir/wd_grammar.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/mydev/myfiles/library/grammar/wd_grammar.cpp > CMakeFiles/grammar.dir/wd_grammar.cpp.i

CMakeFiles/grammar.dir/wd_grammar.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/grammar.dir/wd_grammar.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/mydev/myfiles/library/grammar/wd_grammar.cpp -o CMakeFiles/grammar.dir/wd_grammar.cpp.s

CMakeFiles/grammar.dir/wd_grammar.cpp.o.requires:

.PHONY : CMakeFiles/grammar.dir/wd_grammar.cpp.o.requires

CMakeFiles/grammar.dir/wd_grammar.cpp.o.provides: CMakeFiles/grammar.dir/wd_grammar.cpp.o.requires
	$(MAKE) -f CMakeFiles/grammar.dir/build.make CMakeFiles/grammar.dir/wd_grammar.cpp.o.provides.build
.PHONY : CMakeFiles/grammar.dir/wd_grammar.cpp.o.provides

CMakeFiles/grammar.dir/wd_grammar.cpp.o.provides.build: CMakeFiles/grammar.dir/wd_grammar.cpp.o


CMakeFiles/grammar.dir/linuxhttp/LinuxSocket.cpp.o: CMakeFiles/grammar.dir/flags.make
CMakeFiles/grammar.dir/linuxhttp/LinuxSocket.cpp.o: ../linuxhttp/LinuxSocket.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/mydev/myfiles/library/grammar/cmake-build-debug-bundled/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/grammar.dir/linuxhttp/LinuxSocket.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/grammar.dir/linuxhttp/LinuxSocket.cpp.o -c /root/mydev/myfiles/library/grammar/linuxhttp/LinuxSocket.cpp

CMakeFiles/grammar.dir/linuxhttp/LinuxSocket.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/grammar.dir/linuxhttp/LinuxSocket.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/mydev/myfiles/library/grammar/linuxhttp/LinuxSocket.cpp > CMakeFiles/grammar.dir/linuxhttp/LinuxSocket.cpp.i

CMakeFiles/grammar.dir/linuxhttp/LinuxSocket.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/grammar.dir/linuxhttp/LinuxSocket.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/mydev/myfiles/library/grammar/linuxhttp/LinuxSocket.cpp -o CMakeFiles/grammar.dir/linuxhttp/LinuxSocket.cpp.s

CMakeFiles/grammar.dir/linuxhttp/LinuxSocket.cpp.o.requires:

.PHONY : CMakeFiles/grammar.dir/linuxhttp/LinuxSocket.cpp.o.requires

CMakeFiles/grammar.dir/linuxhttp/LinuxSocket.cpp.o.provides: CMakeFiles/grammar.dir/linuxhttp/LinuxSocket.cpp.o.requires
	$(MAKE) -f CMakeFiles/grammar.dir/build.make CMakeFiles/grammar.dir/linuxhttp/LinuxSocket.cpp.o.provides.build
.PHONY : CMakeFiles/grammar.dir/linuxhttp/LinuxSocket.cpp.o.provides

CMakeFiles/grammar.dir/linuxhttp/LinuxSocket.cpp.o.provides.build: CMakeFiles/grammar.dir/linuxhttp/LinuxSocket.cpp.o


CMakeFiles/grammar.dir/mylog/MyLog.cpp.o: CMakeFiles/grammar.dir/flags.make
CMakeFiles/grammar.dir/mylog/MyLog.cpp.o: ../mylog/MyLog.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/mydev/myfiles/library/grammar/cmake-build-debug-bundled/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/grammar.dir/mylog/MyLog.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/grammar.dir/mylog/MyLog.cpp.o -c /root/mydev/myfiles/library/grammar/mylog/MyLog.cpp

CMakeFiles/grammar.dir/mylog/MyLog.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/grammar.dir/mylog/MyLog.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/mydev/myfiles/library/grammar/mylog/MyLog.cpp > CMakeFiles/grammar.dir/mylog/MyLog.cpp.i

CMakeFiles/grammar.dir/mylog/MyLog.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/grammar.dir/mylog/MyLog.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/mydev/myfiles/library/grammar/mylog/MyLog.cpp -o CMakeFiles/grammar.dir/mylog/MyLog.cpp.s

CMakeFiles/grammar.dir/mylog/MyLog.cpp.o.requires:

.PHONY : CMakeFiles/grammar.dir/mylog/MyLog.cpp.o.requires

CMakeFiles/grammar.dir/mylog/MyLog.cpp.o.provides: CMakeFiles/grammar.dir/mylog/MyLog.cpp.o.requires
	$(MAKE) -f CMakeFiles/grammar.dir/build.make CMakeFiles/grammar.dir/mylog/MyLog.cpp.o.provides.build
.PHONY : CMakeFiles/grammar.dir/mylog/MyLog.cpp.o.provides

CMakeFiles/grammar.dir/mylog/MyLog.cpp.o.provides.build: CMakeFiles/grammar.dir/mylog/MyLog.cpp.o


CMakeFiles/grammar.dir/method/Method.cpp.o: CMakeFiles/grammar.dir/flags.make
CMakeFiles/grammar.dir/method/Method.cpp.o: ../method/Method.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/mydev/myfiles/library/grammar/cmake-build-debug-bundled/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/grammar.dir/method/Method.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/grammar.dir/method/Method.cpp.o -c /root/mydev/myfiles/library/grammar/method/Method.cpp

CMakeFiles/grammar.dir/method/Method.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/grammar.dir/method/Method.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/mydev/myfiles/library/grammar/method/Method.cpp > CMakeFiles/grammar.dir/method/Method.cpp.i

CMakeFiles/grammar.dir/method/Method.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/grammar.dir/method/Method.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/mydev/myfiles/library/grammar/method/Method.cpp -o CMakeFiles/grammar.dir/method/Method.cpp.s

CMakeFiles/grammar.dir/method/Method.cpp.o.requires:

.PHONY : CMakeFiles/grammar.dir/method/Method.cpp.o.requires

CMakeFiles/grammar.dir/method/Method.cpp.o.provides: CMakeFiles/grammar.dir/method/Method.cpp.o.requires
	$(MAKE) -f CMakeFiles/grammar.dir/build.make CMakeFiles/grammar.dir/method/Method.cpp.o.provides.build
.PHONY : CMakeFiles/grammar.dir/method/Method.cpp.o.provides

CMakeFiles/grammar.dir/method/Method.cpp.o.provides.build: CMakeFiles/grammar.dir/method/Method.cpp.o


# Object files for target grammar
grammar_OBJECTS = \
"CMakeFiles/grammar.dir/main.cpp.o" \
"CMakeFiles/grammar.dir/wd_grammar.cpp.o" \
"CMakeFiles/grammar.dir/linuxhttp/LinuxSocket.cpp.o" \
"CMakeFiles/grammar.dir/mylog/MyLog.cpp.o" \
"CMakeFiles/grammar.dir/method/Method.cpp.o"

# External object files for target grammar
grammar_EXTERNAL_OBJECTS =

grammar: CMakeFiles/grammar.dir/main.cpp.o
grammar: CMakeFiles/grammar.dir/wd_grammar.cpp.o
grammar: CMakeFiles/grammar.dir/linuxhttp/LinuxSocket.cpp.o
grammar: CMakeFiles/grammar.dir/mylog/MyLog.cpp.o
grammar: CMakeFiles/grammar.dir/method/Method.cpp.o
grammar: CMakeFiles/grammar.dir/build.make
grammar: CMakeFiles/grammar.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/mydev/myfiles/library/grammar/cmake-build-debug-bundled/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable grammar"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/grammar.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/grammar.dir/build: grammar

.PHONY : CMakeFiles/grammar.dir/build

CMakeFiles/grammar.dir/requires: CMakeFiles/grammar.dir/main.cpp.o.requires
CMakeFiles/grammar.dir/requires: CMakeFiles/grammar.dir/wd_grammar.cpp.o.requires
CMakeFiles/grammar.dir/requires: CMakeFiles/grammar.dir/linuxhttp/LinuxSocket.cpp.o.requires
CMakeFiles/grammar.dir/requires: CMakeFiles/grammar.dir/mylog/MyLog.cpp.o.requires
CMakeFiles/grammar.dir/requires: CMakeFiles/grammar.dir/method/Method.cpp.o.requires

.PHONY : CMakeFiles/grammar.dir/requires

CMakeFiles/grammar.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/grammar.dir/cmake_clean.cmake
.PHONY : CMakeFiles/grammar.dir/clean

CMakeFiles/grammar.dir/depend:
	cd /root/mydev/myfiles/library/grammar/cmake-build-debug-bundled && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/mydev/myfiles/library/grammar /root/mydev/myfiles/library/grammar /root/mydev/myfiles/library/grammar/cmake-build-debug-bundled /root/mydev/myfiles/library/grammar/cmake-build-debug-bundled /root/mydev/myfiles/library/grammar/cmake-build-debug-bundled/CMakeFiles/grammar.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/grammar.dir/depend
