if (USE_GENERAL)
    file(GLOB general_srcs ${CMAKE_CURRENT_SOURCE_DIR}/cpu/general/*.cpp)
endif (USE_GENERAL)

if (USE_NEON)
    if (USE_FP16)
        file(GLOB arm_fp16_srcs ${CMAKE_CURRENT_SOURCE_DIR}/cpu/arm/fp16/*.cpp)
    endif (USE_FP16)
    if (USE_FP32)
        file(GLOB arm_fp32_srcs ${CMAKE_CURRENT_SOURCE_DIR}/cpu/arm/fp32/*.cpp)
    endif (USE_FP32)
    if (USE_INT8)
        file(GLOB arm_int8_srcs ${CMAKE_CURRENT_SOURCE_DIR}/cpu/arm/int8/*.cpp)
    endif (USE_INT8)
    file(GLOB arm_srcs ${CMAKE_CURRENT_SOURCE_DIR}/cpu/arm/*.cpp)
    set(arm_srcs "${arm_srcs};${arm_fp16_srcs};${arm_fp32_srcs};${arm_int8_srcs}")
endif (USE_NEON)

file(GLOB srcs ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
set(srcs "${srcs};${general_srcs};${arm_srcs}")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# shared library 
ADD_LIBRARY(${PROJECT_NAME} SHARED ${srcs}) 

# static library 
ADD_LIBRARY(${PROJECT_NAME}_static STATIC ${srcs}) 

SET_TARGET_PROPERTIES(${PROJECT_NAME}_static PROPERTIES OUTPUT_NAME "${PROJECT_NAME}") 
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES CLEAN_DIRECT_OUTPUT 1) 
SET_TARGET_PROPERTIES(${PROJECT_NAME}_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
