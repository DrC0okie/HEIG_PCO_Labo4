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
if(NOT Qt5_FOUND)
    message(FATAL_ERROR "Qt 5 is required.")
endif()

# add_compile_definitions(NOSOUND)           # Compile without link to sound when loco has an accident
# add_compile_definitions(MAQUETTE)          # Use maquettes instead of sim
# add_compile_definitions(CDEVELOP)          # Use cmain instead of cppmain
# add_compile_definitions(DRAW_BOUNDINGRECT) # Use alternate painting style

# Add Qt modules
if(NOT NOSOUND)
    find_package(Qt5 COMPONENTS Multimedia REQUIRED)
    add_compile_definitions(WITHSOUND)
endif()

# Target and template
#set(TARGET_NAME QtrainSim)
#set(TEMPLATE app)

# Configuration
set(CMAKE_BUILD_TYPE Debug)

# Install maquettes files
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/QtrainSim/data/Maquettes DESTINATION dist/data)

# Install file containing the infos about tracks
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/QtrainSim/data/infosVoies.txt DESTINATION dist/data)

# Define platform-specific settings
if(WIN32)
    add_compile_definitions(ON_WINDOWS)
    set(UI_DIR tmp/win/ui)
    set(MOC_DIR tmp/win/moc)
    set(OBJECTS_DIR tmp/win/obj)
    set(RCC_DIR tmp/win/rcc)
    if(CDEVELOP)
        target_link_libraries(QtrainSim -lpthread)
    endif()
elseif(UNIX)
    add_compile_definitions(ON_LINUX)
    set(UI_DIR tmp/linux/ui)
    set(MOC_DIR tmp/linux/moc)
    set(OBJECTS_DIR tmp/linux/obj)
    set(RCC_DIR tmp/linux/rcc)
elseif(APPLE)
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
endif()