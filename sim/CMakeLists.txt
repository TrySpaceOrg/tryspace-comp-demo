cmake_minimum_required(VERSION 3.25)
project(demo_sim C)

include(../../CompFlags.cmake)

# Find ZeroMQ dependency
find_package(PkgConfig REQUIRED)
pkg_check_modules(ZeroMQ REQUIRED libzmq)

  # Find Simulith headers
set(SIMULITH_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../simulith" CACHE PATH "Path to Simulith directory")
if(NOT EXISTS "${SIMULITH_DIR}/include/simulith_uart.h")
    message(FATAL_ERROR "Could not find simulith_uart.h in ${SIMULITH_DIR}/include. Please set SIMULITH_DIR to the correct path.")
endif()

# Only add Simulith subdirectory if not already added
if(NOT TARGET simulith)
# Set policy to NEW to allow external project linking
cmake_policy(SET CMP0079 NEW)
add_subdirectory(${SIMULITH_DIR} ${CMAKE_BINARY_DIR}/simulith)
endif()



# Create the standalone executable
add_executable(demo_sim
    demo_sim.c
    ../shared/demo_device.c
    ${SIMULITH_DIR}/src/simulith_uart.c
    ${SIMULITH_DIR}/src/simulith_common.c
    ../../../fsw/apps/hwlib/sim/src/libuart.c
)

# Create the shared library for dynamic loading by director
add_library(demo_sim_component SHARED
    demo_sim.c
    ../shared/demo_device.c
    ${SIMULITH_DIR}/src/simulith_uart.c
    ${SIMULITH_DIR}/src/simulith_common.c
    ../../../fsw/apps/hwlib/sim/src/libuart.c
)

# Include directories for both targets
target_include_directories(demo_sim
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${SIMULITH_DIR}/include
    ../shared
    ../../../fsw/apps/hwlib/fsw/public_inc
    ${ZeroMQ_INCLUDE_DIRS}
)

target_include_directories(demo_sim_component
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${SIMULITH_DIR}/include
    ../shared
    ../../../fsw/apps/hwlib/fsw/public_inc
    ${ZeroMQ_INCLUDE_DIRS}
)

# Compile definitions
target_compile_definitions(demo_sim PRIVATE STANDALONE_MODE)
# demo_sim_component gets no STANDALONE_MODE definition

# Link against Simulith and ZeroMQ for both targets
target_link_libraries(demo_sim
    PRIVATE
    simulith
    ${ZeroMQ_LIBRARIES}
)

target_link_libraries(demo_sim_component
    PRIVATE
    simulith
    ${ZeroMQ_LIBRARIES}
)

# Set C standard for both targets
set_target_properties(demo_sim PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED ON
)

set_target_properties(demo_sim_component PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED ON
    OUTPUT_NAME "demo_sim"
    PREFIX ""
)

# Add dependencies on simulith for both targets
add_dependencies(demo_sim simulith)
add_dependencies(demo_sim_component simulith)
