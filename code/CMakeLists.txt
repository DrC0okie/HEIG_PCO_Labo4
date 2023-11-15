cmake_minimum_required(VERSION 3.25)

project(StudentProject)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS False)

# Find the required Qt components
find_package(Qt5 COMPONENTS Core Widgets PrintSupport REQUIRED)

# Find the PcoSyncrho library
# TODO

# Add your source files
file(GLOB SOURCES src/*.cpp)

# Add executable target
add_executable(StudentProject ${SOURCES})

# Include directories for header files
#target_include_directories(StudentProject PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Link against the required Qt modules
target_link_libraries(StudentProject PRIVATE Qt5::Core Qt5::Widgets Qt5::PrintSupport)

# Link against the QTrainSim library (make sure it's built first) and PcoSyncrho
target_link_libraries(StudentProject PUBLIC QtrainSim pcosynchro)