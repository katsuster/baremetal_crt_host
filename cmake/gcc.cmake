# SPDX-License-Identifier: Apache-2.0

# Common
prj_compile_options(-g -Wall)

# Assembler
prj_compile_options($<$<COMPILE_LANGUAGE:ASM>:-DASMLANG>)
