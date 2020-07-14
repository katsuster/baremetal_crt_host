set(ICD_SRC
    cl_icd.c
    cl_ext.c)

add_library(testcl SHARED ${ICD_SRC})
