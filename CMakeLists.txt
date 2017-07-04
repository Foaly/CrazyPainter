cmake_minimum_required(VERSION 3.1)
set(PROJECT_NAME "CrazyPainter")
project(${PROJECT_NAME})

# include helpers
include(${CMAKE_CURRENT_SOURCE_DIR}/helpers.cmake)

# Enable debug symbols by default
if(CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE Debug)
endif()

message(STATUS "Configuring in ${CMAKE_BUILD_TYPE} mode.")

# enable C++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# define sources
set(EXECUTABLE_NAME "${PROJECT_NAME}")
include_directories("src/*")
set(SOURCE_FILES src/CircleInterpolation.cpp
                 src/CircleInterpolation.hpp
                 src/ColorGenerator.cpp
                 src/ColorGenerator.hpp
                 src/CrazyPainter.cpp
                 src/CrazyPainter.hpp
                 src/HermiteInterpolation.cpp
                 src/HermiteInterpolation.hpp
                 src/InterpolationMode.hpp
                 src/JitterMode.cpp
                 src/JitterMode.hpp
                 src/main.cpp
                 src/Random.cpp
                 src/Random.hpp
                 src/RoundendedLine.cpp
                 src/RoundendedLine.hpp
                 src/SmoothstepInterpolation.cpp
                 src/SmoothstepInterpolation.hpp
                 src/Utilities.hpp
                 src/PathResolver.hpp
                 src/StarInterpolation.hpp
                 src/StarInterpolation.cpp
                 src/DeflectInterpolation.hpp
                 src/DeflectInterpolation.cpp
                 src/SpiralInterpolation.hpp
                 src/SpiralInterpolation.cpp
                 )

# when we are on MacOS create a bundle and
# put the rescources in it
if(APPLE)
    set(SOURCE_FILES ${SOURCE_FILES}
                     src/osx/PathResolver.mm)

    add_executable(${EXECUTABLE_NAME} MACOSX_BUNDLE ${SOURCE_FILES})

    # add .plist file
    set_target_properties(${EXECUTABLE_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/src/osx/Info.plist.in)
else()
    # on linux/windows there are no bundles
    set(SOURCE_FILES ${SOURCE_FILES}
                     src/PathResolver.cpp)

    add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})
endif()

# copy resource files
copy_resource_folder_next_to_target(${EXECUTABLE_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/data data)
copy_resource_folder_next_to_target(${EXECUTABLE_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/logs logs)


# Add our CMake scripts
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

# Detect and add any version 2.X of SFML
# See the FindSFML.cmake file for additional details and instructions
find_package(SFML 2 COMPONENTS system window graphics network audio REQUIRED)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})
else()
	  message(FATAL_ERROR "Unable to find SFML!")
endif()

# Link to OpenGL
find_package(OpenGL)
if(OPENGL_FOUND)
    include_directories(${OPENGL_INCLUDE_DIR})
    target_link_libraries(${EXECUTABLE_NAME} ${OPENGL_LIBRARIES})
else()
	  message(FATAL_ERROR "Unable to find OpenGL!")
endif()
