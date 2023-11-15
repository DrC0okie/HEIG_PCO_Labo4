cmake_minimum_required(VERSION 3.25)

project(StudentProject)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS False)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wconversion -Wsign-conversion -pedantic")

# Find the required Qt components
find_package(Qt5 COMPONENTS Core Widgets PrintSupport REQUIRED)

# Find the PcoSyncrho library
# TODO

# Add your source files
file(GLOB SOURCES src/*.cpp)

# Add executable target
add_executable(StudentProject ${SOURCES})

# Configure compiler flags
if (CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG)
    target_compile_options(${PROJECT_NAME} PRIVATE -g3 -fPIE -pie)
elseif (CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_definitions(${PROJECT_NAME} PRIVATE NDEBUG)
    target_compile_options(${PROJECT_NAME} PRIVATE -Ofast)
endif ()

# Link against the required Qt modules
target_link_libraries(StudentProject PRIVATE Qt5::Core Qt5::Widgets Qt5::PrintSupport)

# Link against the QTrainSim library (make sure it's built first) and PcoSyncrho
target_link_libraries(StudentProject PUBLIC QtrainSim pcosynchro)