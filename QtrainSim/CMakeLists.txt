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

# Install maquettes files
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/data/Maquettes DESTINATION ${CMAKE_BINARY_DIR}/code/data)

# Install file containing the infos about tracks
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/data/infosVoies.txt DESTINATION ${CMAKE_BINARY_DIR}/code/data)

# Define platform-specific settings
if (WIN32)
    add_compile_definitions(ON_WINDOWS)
    set(UI_DIR tmp/win/ui)
    set(MOC_DIR tmp/win/moc)
    set(OBJECTS_DIR tmp/win/obj)
    set(RCC_DIR tmp/win/rcc)
    if (CDEVELOP)
        target_link_libraries(QtrainSim -lpthread)
    endif ()
elseif (UNIX)
    add_compile_definitions(ON_LINUX)
    set(UI_DIR tmp/linux/ui)
    set(MOC_DIR tmp/linux/moc)
    set(OBJECTS_DIR tmp/linux/obj)
    set(RCC_DIR tmp/linux/rcc)
elseif (APPLE)
    add_compile_definitions(ON_MACOS)
    set(UI_DIR tmp/mac/ui)
    set(MOC_DIR tmp/mac/moc)
    set(OBJECTS_DIR tmp/mac/obj)
    set(RCC_DIR tmp/mac/rcc)
    # Avoid creating a bundle that causes troubles with the maquette files
    set(CMAKE_CONFIGURE_OPTIONS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_MACOSX_BUNDLE OFF)
    remove_definitions(-DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES})
    remove_definitions(-DCMAKE_OSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET})
    remove_definitions(-DCMAKE_OSX_SYSROOT=${CMAKE_OSX_SYSROOT})
    remove_definitions(-DCMAKE_CXX_FLAGS_MINSIZEREL=${CMAKE_CXX_FLAGS_MINSIZEREL})
    remove_definitions(-DCMAKE_C_FLAGS_MINSIZEREL=${CMAKE_C_FLAGS_MINSIZEREL})
    remove_definitions(-DCMAKE_EXE_LINKER_FLAGS_MINSIZEREL=${CMAKE_EXE_LINKER_FLAGS_MINSIZEREL})
    remove_definitions(-DCMAKE_SHARED_LINKER_FLAGS_MINSIZEREL=${CMAKE_SHARED_LINKER_FLAGS_MINSIZEREL})
endif ()