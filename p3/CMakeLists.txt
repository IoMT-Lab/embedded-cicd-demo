cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME P3)

# Include toolchain file
include("cmake/gcc-arm-none-eabi.cmake")

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

add_compile_options(-Werror -Wno-incompatible-pointer-types)  # Treat warnings as errors

set(TEST_EXECUTABLE ${CMAKE_PROJECT_NAME}_unit_tests)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})
add_executable(${TEST_EXECUTABLE})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

set(SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/utils/serial.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/utils/test_harness.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/app/app.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/app/as7341.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/testing/tests.c
)

set(INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/Inc/system
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${SOURCE})
target_sources(${TEST_EXECUTABLE} PRIVATE ${SOURCE})

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${INCLUDES})
target_include_directories(${TEST_EXECUTABLE} PRIVATE ${INCLUDES})

# Add project symbols (macros)
target_compile_definitions(${TEST_EXECUTABLE} PRIVATE
    UNIT_TESTING
    # Add user defined symbols
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME} stm32cubemx)
target_link_libraries(${TEST_EXECUTABLE} stm32cubemx)

