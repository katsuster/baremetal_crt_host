# SPDX-License-Identifier: Apache-2.0

# Common
prj_compile_options(
  -Wall
  $<$<CONFIG:Debug>:>
  $<$<CONFIG:Release>:>
  $<$<CONFIG:MinSizeRel>:>
  $<$<CONFIG:RelWithDebInfo>:>
)

# Assembler
prj_compile_options($<$<COMPILE_LANGUAGE:ASM>:-DASMLANG>)
