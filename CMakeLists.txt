# SPDX-License-Identifier: Apache-2.0

project(baremetal_crt_host)

cmake_minimum_required(VERSION 3.13)
enable_language(C)

# Common settings
set(PRJ_LIB_NAME bmetal)
set(PRJ_CONFIG_NAME config_cur)
set(PRJ_CONFIG_CACHE_NAME config_cache)
set(PRJ_CONFIG_NOTIFY_NAME config_notify.cmake)
set(PRJ_DEFCONFIG_NAME defconfig)
set(PRJ_AUTOCONF_H_DIR include/generated)
set(PRJ_AUTOCONF_H_NAME autoconf.h)

# CMake basic tools to add/use directories, sources and configs
include(${CMAKE_SOURCE_DIR}/cmake/tools.cmake)

# Target: Library
add_library(${PRJ_LIB_NAME} SHARED)
target_link_libraries(${PRJ_LIB_NAME})

# Use Kconfig like configuration system.
# After that, we can use CONFIG_ABCD variables in CMake
include(${CMAKE_SOURCE_DIR}/cmake/kconfig.cmake)

# Compiler settings
if("${CC}" STREQUAL "gcc")
  include(${CMAKE_SOURCE_DIR}/cmake/gcc.cmake)
endif()

# Install, include settings
prj_include_directories(
  PUBLIC ${CMAKE_SOURCE_DIR}/include
  PRIVATE ${CMAKE_BINARY_DIR}/include
  )

add_subdirectory(drivers)
add_subdirectory(src)
