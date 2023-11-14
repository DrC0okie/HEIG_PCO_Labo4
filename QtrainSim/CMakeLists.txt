set(LIB_NAME QtrainSim)

# QT Directives
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Add the source and header files for the library
file(GLOB LIB_SOURCES src/*.cpp)
file(GLOB LIB_HEADERS src/*.h)

# Create the library
add_library(${LIB_NAME} SHARED ${LIB_HEADERS} ${LIB_SOURCES})

# Include directories
target_include_directories(${LIB_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/QtrainSim/src>)

# Link necessary libraries (Qt modules)
find_package(Qt5 COMPONENTS Core Gui Widgets PrintSupport REQUIRED)
target_link_libraries(${LIB_NAME} Qt5::Core Qt5::Gui Qt5::Widgets Qt5::PrintSupport)

# Set the C++ standard
set_property(TARGET ${LIB_NAME} PROPERTY CXX_STANDARD 17)