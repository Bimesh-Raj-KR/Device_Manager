
# Generate linux executable
cmake_minimum_required(VERSION 3.10)
project(GccBuild C)

include_directories(
    ${CMAKE_SOURCE_DIR}/
    ${CMAKE_SOURCE_DIR}/fileOperations
    ${CMAKE_SOURCE_DIR}/deviceManager
    ${CMAKE_SOURCE_DIR}/cJSON)

# In device directory
add_executable(device_gcc
    ${CMAKE_SOURCE_DIR}/main.c
    ${CMAKE_SOURCE_DIR}/fileOperations/fileOperations.c
    ${CMAKE_SOURCE_DIR}/deviceManager/deviceManager.c
    ${CMAKE_SOURCE_DIR}/cJSON/cJSON.c)

target_compile_options(device_gcc PRIVATE -Wall -Wextra -Werror 
                    -fanalyzer)

set_target_properties(device_gcc PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/device)

# In debug directory for debugging
add_executable(device_debug
   ${CMAKE_SOURCE_DIR}/main.c
    ${CMAKE_SOURCE_DIR}/fileOperations/fileOperations.c
    ${CMAKE_SOURCE_DIR}/deviceManager/deviceManager.c
    ${CMAKE_SOURCE_DIR}/cJSON/cJSON.c)

target_compile_options(device_debug PRIVATE -g -O0)

set_target_properties(device_debug PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/debug)

