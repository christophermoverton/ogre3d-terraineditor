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

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/cmake-gui

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/strangequark2/ogre3d-terraineditor/clean_ogre_cmake_project

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/strangequark2/ogre3d-terraineditor/clean_ogre_cmake_project

# Include any dependencies generated for this target.
include CMakeFiles/OgreApp.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/OgreApp.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/OgreApp.dir/flags.make

CMakeFiles/OgreApp.dir/BaseApplication.cpp.o: CMakeFiles/OgreApp.dir/flags.make
CMakeFiles/OgreApp.dir/BaseApplication.cpp.o: BaseApplication.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/strangequark2/ogre3d-terraineditor/clean_ogre_cmake_project/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/OgreApp.dir/BaseApplication.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/OgreApp.dir/BaseApplication.cpp.o -c /home/strangequark2/ogre3d-terraineditor/clean_ogre_cmake_project/BaseApplication.cpp

CMakeFiles/OgreApp.dir/BaseApplication.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/OgreApp.dir/BaseApplication.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/strangequark2/ogre3d-terraineditor/clean_ogre_cmake_project/BaseApplication.cpp > CMakeFiles/OgreApp.dir/BaseApplication.cpp.i

CMakeFiles/OgreApp.dir/BaseApplication.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/OgreApp.dir/BaseApplication.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/strangequark2/ogre3d-terraineditor/clean_ogre_cmake_project/BaseApplication.cpp -o CMakeFiles/OgreApp.dir/BaseApplication.cpp.s

CMakeFiles/OgreApp.dir/BaseApplication.cpp.o.requires:
.PHONY : CMakeFiles/OgreApp.dir/BaseApplication.cpp.o.requires

CMakeFiles/OgreApp.dir/BaseApplication.cpp.o.provides: CMakeFiles/OgreApp.dir/BaseApplication.cpp.o.requires
	$(MAKE) -f CMakeFiles/OgreApp.dir/build.make CMakeFiles/OgreApp.dir/BaseApplication.cpp.o.provides.build
.PHONY : CMakeFiles/OgreApp.dir/BaseApplication.cpp.o.provides

CMakeFiles/OgreApp.dir/BaseApplication.cpp.o.provides.build: CMakeFiles/OgreApp.dir/BaseApplication.cpp.o

CMakeFiles/OgreApp.dir/ITutorial02.cpp.o: CMakeFiles/OgreApp.dir/flags.make
CMakeFiles/OgreApp.dir/ITutorial02.cpp.o: ITutorial02.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/strangequark2/ogre3d-terraineditor/clean_ogre_cmake_project/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/OgreApp.dir/ITutorial02.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/OgreApp.dir/ITutorial02.cpp.o -c /home/strangequark2/ogre3d-terraineditor/clean_ogre_cmake_project/ITutorial02.cpp

CMakeFiles/OgreApp.dir/ITutorial02.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/OgreApp.dir/ITutorial02.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/strangequark2/ogre3d-terraineditor/clean_ogre_cmake_project/ITutorial02.cpp > CMakeFiles/OgreApp.dir/ITutorial02.cpp.i

CMakeFiles/OgreApp.dir/ITutorial02.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/OgreApp.dir/ITutorial02.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/strangequark2/ogre3d-terraineditor/clean_ogre_cmake_project/ITutorial02.cpp -o CMakeFiles/OgreApp.dir/ITutorial02.cpp.s

CMakeFiles/OgreApp.dir/ITutorial02.cpp.o.requires:
.PHONY : CMakeFiles/OgreApp.dir/ITutorial02.cpp.o.requires

CMakeFiles/OgreApp.dir/ITutorial02.cpp.o.provides: CMakeFiles/OgreApp.dir/ITutorial02.cpp.o.requires
	$(MAKE) -f CMakeFiles/OgreApp.dir/build.make CMakeFiles/OgreApp.dir/ITutorial02.cpp.o.provides.build
.PHONY : CMakeFiles/OgreApp.dir/ITutorial02.cpp.o.provides

CMakeFiles/OgreApp.dir/ITutorial02.cpp.o.provides.build: CMakeFiles/OgreApp.dir/ITutorial02.cpp.o

# Object files for target OgreApp
OgreApp_OBJECTS = \
"CMakeFiles/OgreApp.dir/BaseApplication.cpp.o" \
"CMakeFiles/OgreApp.dir/ITutorial02.cpp.o"

# External object files for target OgreApp
OgreApp_EXTERNAL_OBJECTS =

dist/bin/OgreApp: CMakeFiles/OgreApp.dir/BaseApplication.cpp.o
dist/bin/OgreApp: CMakeFiles/OgreApp.dir/ITutorial02.cpp.o
dist/bin/OgreApp: CMakeFiles/OgreApp.dir/build.make
dist/bin/OgreApp: /usr/local/lib/libOgreMain.so
dist/bin/OgreApp: /usr/lib/x86_64-linux-gnu/libboost_thread.so
dist/bin/OgreApp: /usr/lib/x86_64-linux-gnu/libboost_date_time.so
dist/bin/OgreApp: /usr/lib/x86_64-linux-gnu/libboost_system.so
dist/bin/OgreApp: /usr/lib/x86_64-linux-gnu/libboost_atomic.so
dist/bin/OgreApp: /usr/lib/x86_64-linux-gnu/libboost_chrono.so
dist/bin/OgreApp: /usr/lib/x86_64-linux-gnu/libpthread.so
dist/bin/OgreApp: /usr/local/lib/libOgreTerrain.so
dist/bin/OgreApp: /usr/lib/x86_64-linux-gnu/libOIS.so
dist/bin/OgreApp: /usr/local/lib/libOgreOverlay.so
dist/bin/OgreApp: /usr/local/lib/libCEGUIBase-0.so
dist/bin/OgreApp: /usr/local/lib/libCEGUIOgreRenderer-0.so
dist/bin/OgreApp: Voronoi/libVoronoi.a
dist/bin/OgreApp: Simplex/libSimplex.a
dist/bin/OgreApp: libfbm/liblibfbm.a
dist/bin/OgreApp: DiamondSquare/libDiamondSquare.a
dist/bin/OgreApp: /usr/local/lib/libfftw3.a
dist/bin/OgreApp: CMakeFiles/OgreApp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable dist/bin/OgreApp"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/OgreApp.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/OgreApp.dir/build: dist/bin/OgreApp
.PHONY : CMakeFiles/OgreApp.dir/build

# Object files for target OgreApp
OgreApp_OBJECTS = \
"CMakeFiles/OgreApp.dir/BaseApplication.cpp.o" \
"CMakeFiles/OgreApp.dir/ITutorial02.cpp.o"

# External object files for target OgreApp
OgreApp_EXTERNAL_OBJECTS =

CMakeFiles/CMakeRelink.dir/OgreApp: CMakeFiles/OgreApp.dir/BaseApplication.cpp.o
CMakeFiles/CMakeRelink.dir/OgreApp: CMakeFiles/OgreApp.dir/ITutorial02.cpp.o
CMakeFiles/CMakeRelink.dir/OgreApp: CMakeFiles/OgreApp.dir/build.make
CMakeFiles/CMakeRelink.dir/OgreApp: /usr/local/lib/libOgreMain.so
CMakeFiles/CMakeRelink.dir/OgreApp: /usr/lib/x86_64-linux-gnu/libboost_thread.so
CMakeFiles/CMakeRelink.dir/OgreApp: /usr/lib/x86_64-linux-gnu/libboost_date_time.so
CMakeFiles/CMakeRelink.dir/OgreApp: /usr/lib/x86_64-linux-gnu/libboost_system.so
CMakeFiles/CMakeRelink.dir/OgreApp: /usr/lib/x86_64-linux-gnu/libboost_atomic.so
CMakeFiles/CMakeRelink.dir/OgreApp: /usr/lib/x86_64-linux-gnu/libboost_chrono.so
CMakeFiles/CMakeRelink.dir/OgreApp: /usr/lib/x86_64-linux-gnu/libpthread.so
CMakeFiles/CMakeRelink.dir/OgreApp: /usr/local/lib/libOgreTerrain.so
CMakeFiles/CMakeRelink.dir/OgreApp: /usr/lib/x86_64-linux-gnu/libOIS.so
CMakeFiles/CMakeRelink.dir/OgreApp: /usr/local/lib/libOgreOverlay.so
CMakeFiles/CMakeRelink.dir/OgreApp: /usr/local/lib/libCEGUIBase-0.so
CMakeFiles/CMakeRelink.dir/OgreApp: /usr/local/lib/libCEGUIOgreRenderer-0.so
CMakeFiles/CMakeRelink.dir/OgreApp: Voronoi/libVoronoi.a
CMakeFiles/CMakeRelink.dir/OgreApp: Simplex/libSimplex.a
CMakeFiles/CMakeRelink.dir/OgreApp: libfbm/liblibfbm.a
CMakeFiles/CMakeRelink.dir/OgreApp: DiamondSquare/libDiamondSquare.a
CMakeFiles/CMakeRelink.dir/OgreApp: /usr/local/lib/libfftw3.a
CMakeFiles/CMakeRelink.dir/OgreApp: CMakeFiles/OgreApp.dir/relink.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable CMakeFiles/CMakeRelink.dir/OgreApp"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/OgreApp.dir/relink.txt --verbose=$(VERBOSE)

# Rule to relink during preinstall.
CMakeFiles/OgreApp.dir/preinstall: CMakeFiles/CMakeRelink.dir/OgreApp
.PHONY : CMakeFiles/OgreApp.dir/preinstall

CMakeFiles/OgreApp.dir/requires: CMakeFiles/OgreApp.dir/BaseApplication.cpp.o.requires
CMakeFiles/OgreApp.dir/requires: CMakeFiles/OgreApp.dir/ITutorial02.cpp.o.requires
.PHONY : CMakeFiles/OgreApp.dir/requires

CMakeFiles/OgreApp.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/OgreApp.dir/cmake_clean.cmake
.PHONY : CMakeFiles/OgreApp.dir/clean

CMakeFiles/OgreApp.dir/depend:
	cd /home/strangequark2/ogre3d-terraineditor/clean_ogre_cmake_project && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/strangequark2/ogre3d-terraineditor/clean_ogre_cmake_project /home/strangequark2/ogre3d-terraineditor/clean_ogre_cmake_project /home/strangequark2/ogre3d-terraineditor/clean_ogre_cmake_project /home/strangequark2/ogre3d-terraineditor/clean_ogre_cmake_project /home/strangequark2/ogre3d-terraineditor/clean_ogre_cmake_project/CMakeFiles/OgreApp.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/OgreApp.dir/depend

