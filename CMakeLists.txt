# SPDX-License-Identifier: Apache-2.0

project(baremetal_crt_host)

cmake_minimum_required(VERSION 3.13)
enable_language(C)

# Common settings
set(PRJ_LIB_NAME bmetal)

# CMake basic tools to add/use directories, sources and configs
include(${CMAKE_SOURCE_DIR}/cmake/tools.cmake)

# Target: Library
add_library(${PRJ_LIB_NAME} SHARED)
target_link_libraries(${PRJ_LIB_NAME})

# Compiler settings
prj_compile_options(-g)

# Install, include settings
prj_include_directories(
  PUBLIC ${CMAKE_SOURCE_DIR}/include
  )

add_subdirectory(src)
