cmake_minimum_required(VERSION 2.8)

set( PROJ_NAME      "graphics_project" )
set( PROJ_PATH      ${CMAKE_SOURCE_DIR} )
set( PROJ_OUT_PATH  ${CMAKE_BINARY_DIR} )
set( PROJ_SOURCES   "")
set( PROJ_HEADERS   "")

set( CMAKE_MODULE_PATH "${PROJ_PATH}/cmake")
set( EXECUTABLE_OUTPUT_PATH ${PROJ_OUT_PATH})

set( PROJ_LIB_PATH  "${PROJ_PATH}/lib" )
set( PROJ_INCLUDES  "${PROJ_PATH}/src")
set( LIB_INCLUDES "${PROJ_LIB_PATH}/include")

project(${PROJ_NAME})
include(ExternalProject)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#Setup dependencies

#glfw3
ExternalProject_Add(glfw
PREFIX ${PROJ_LIB_PATH}
GIT_REPOSITORY https://github.com/glfw/glfw.git
GIT_TAG tags/3.1
INSTALL_DIR ${PROJ_LIB_PATH}
CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${PROJ_LIB_PATH}
)

#glm
ExternalProject_Add(glm
PREFIX ${PROJ_LIB_PATH}
GIT_REPOSITORY https://github.com/g-truc/glm.git
GIT_TAG tags/0.9.6.3
INSTALL_DIR ${PROJ_LIB_PATH}
CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${PROJ_LIB_PATH}
)

#glew
ExternalProject_Add(glew
PREFIX ${PROJ_LIB_PATH}
GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
GIT_TAG tags/glew-cmake-1.11.0
INSTALL_DIR ${PROJ_LIB_PATH}
CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${PROJ_LIB_PATH}
)

#link libraries
set( GLFW_LIB_PATH "${PROJ_LIB_PATH}/lib/libglfw3.a")
set( GLEW_LIB_PATH "${PROJ_LIB_PATH}/lib/libglew.a")

# Adds sources and headers to project
file( GLOB_RECURSE PROJ_SOURCES src/*.cpp )
file( GLOB_RECURSE PROJ_HEADERS include/*.h src/*.h)
include_directories( ${PROJ_INCLUDES} ${LIB_INCLUDES})

add_subdirectory(src)