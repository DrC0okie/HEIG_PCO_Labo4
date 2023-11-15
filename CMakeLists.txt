cmake_minimum_required(VERSION 3.25)

project(QtrainSim)

# Add the QtrainSim library
add_subdirectory(QtrainSim)

# Add the student project
add_subdirectory(code)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Find Qt5 package
find_package(Qt5 COMPONENTS Core Gui Widgets PrintSupport REQUIRED)
if (NOT Qt5_FOUND)
    message(FATAL_ERROR "Qt 5 is required.")
endif ()

# add_compile_definitions(NOSOUND)           # Compile without link to sound when loco has an accident
# add_compile_definitions(MAQUETTE)          # Use maquettes instead of sim
# add_compile_definitions(CDEVELOP)          # Use cmain instead of cppmain
# add_compile_definitions(DRAW_BOUNDINGRECT) # Use alternate painting style

# Add Qt modules
if (NOT NOSOUND)
    find_package(Qt5 COMPONENTS Multimedia REQUIRED)
    add_compile_definitions(WITHSOUND)
endif ()

# Target and template
#set(TARGET_NAME QtrainSim)
#set(TEMPLATE app)

# Configuration
set(CMAKE_BUILD_TYPE Debug)