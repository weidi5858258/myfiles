# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.12

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
CMAKE_COMMAND = /root/mydev/tools/clion-2018.2.5/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /root/mydev/tools/clion-2018.2.5/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /root/mydev/workspace_github/myfiles/c_plus_plus_project/tcp_ip_client

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/mydev/workspace_github/myfiles/c_plus_plus_project/tcp_ip_client/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/tcp_ip_client.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/tcp_ip_client.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/tcp_ip_client.dir/flags.make

CMakeFiles/tcp_ip_client.dir/main.cpp.o: CMakeFiles/tcp_ip_client.dir/flags.make
CMakeFiles/tcp_ip_client.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/mydev/workspace_github/myfiles/c_plus_plus_project/tcp_ip_client/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/tcp_ip_client.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tcp_ip_client.dir/main.cpp.o -c /root/mydev/workspace_github/myfiles/c_plus_plus_project/tcp_ip_client/main.cpp

CMakeFiles/tcp_ip_client.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tcp_ip_client.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/mydev/workspace_github/myfiles/c_plus_plus_project/tcp_ip_client/main.cpp > CMakeFiles/tcp_ip_client.dir/main.cpp.i

CMakeFiles/tcp_ip_client.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tcp_ip_client.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/mydev/workspace_github/myfiles/c_plus_plus_project/tcp_ip_client/main.cpp -o CMakeFiles/tcp_ip_client.dir/main.cpp.s

CMakeFiles/tcp_ip_client.dir/LinuxSocket.cpp.o: CMakeFiles/tcp_ip_client.dir/flags.make
CMakeFiles/tcp_ip_client.dir/LinuxSocket.cpp.o: ../LinuxSocket.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/mydev/workspace_github/myfiles/c_plus_plus_project/tcp_ip_client/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/tcp_ip_client.dir/LinuxSocket.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tcp_ip_client.dir/LinuxSocket.cpp.o -c /root/mydev/workspace_github/myfiles/c_plus_plus_project/tcp_ip_client/LinuxSocket.cpp

CMakeFiles/tcp_ip_client.dir/LinuxSocket.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tcp_ip_client.dir/LinuxSocket.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/mydev/workspace_github/myfiles/c_plus_plus_project/tcp_ip_client/LinuxSocket.cpp > CMakeFiles/tcp_ip_client.dir/LinuxSocket.cpp.i

CMakeFiles/tcp_ip_client.dir/LinuxSocket.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tcp_ip_client.dir/LinuxSocket.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/mydev/workspace_github/myfiles/c_plus_plus_project/tcp_ip_client/LinuxSocket.cpp -o CMakeFiles/tcp_ip_client.dir/LinuxSocket.cpp.s

# Object files for target tcp_ip_client
tcp_ip_client_OBJECTS = \
"CMakeFiles/tcp_ip_client.dir/main.cpp.o" \
"CMakeFiles/tcp_ip_client.dir/LinuxSocket.cpp.o"

# External object files for target tcp_ip_client
tcp_ip_client_EXTERNAL_OBJECTS =

tcp_ip_client: CMakeFiles/tcp_ip_client.dir/main.cpp.o
tcp_ip_client: CMakeFiles/tcp_ip_client.dir/LinuxSocket.cpp.o
tcp_ip_client: CMakeFiles/tcp_ip_client.dir/build.make
tcp_ip_client: CMakeFiles/tcp_ip_client.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/mydev/workspace_github/myfiles/c_plus_plus_project/tcp_ip_client/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable tcp_ip_client"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tcp_ip_client.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/tcp_ip_client.dir/build: tcp_ip_client

.PHONY : CMakeFiles/tcp_ip_client.dir/build

CMakeFiles/tcp_ip_client.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/tcp_ip_client.dir/cmake_clean.cmake
.PHONY : CMakeFiles/tcp_ip_client.dir/clean

CMakeFiles/tcp_ip_client.dir/depend:
	cd /root/mydev/workspace_github/myfiles/c_plus_plus_project/tcp_ip_client/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/mydev/workspace_github/myfiles/c_plus_plus_project/tcp_ip_client /root/mydev/workspace_github/myfiles/c_plus_plus_project/tcp_ip_client /root/mydev/workspace_github/myfiles/c_plus_plus_project/tcp_ip_client/cmake-build-debug /root/mydev/workspace_github/myfiles/c_plus_plus_project/tcp_ip_client/cmake-build-debug /root/mydev/workspace_github/myfiles/c_plus_plus_project/tcp_ip_client/cmake-build-debug/CMakeFiles/tcp_ip_client.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/tcp_ip_client.dir/depend

