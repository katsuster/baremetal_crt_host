set(ICD_SRC
    cl.c
    cl_icd.c
    cl_ext.c)

add_library(testcl SHARED ${ICD_SRC})
