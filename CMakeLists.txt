# SPDX-License-Identifier: Apache-2.0

project(baremetal_crt_host)

cmake_minimum_required(VERSION 3.13)
enable_language(C)

# Common settings
set(PRJ_LIB_NAME bmetal)

add_library(${PRJ_LIB_NAME} SHARED)

target_include_directories(${PRJ_LIB_NAME}
  PUBLIC
  ${CMAKE_SOURCE_DIR}/include
  )

add_subdirectory(src)

target_link_libraries(${PRJ_LIB_NAME})
