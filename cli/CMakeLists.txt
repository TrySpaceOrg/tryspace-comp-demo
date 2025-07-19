cmake_minimum_required(VERSION 2.6.4)
project (demo_checkout)

if (NOT DEFINED TGTNAME)
  message(FATAL_ERROR "TGTNAME must be defined on the cmake command line (e.g. \"-DTGTNAME=cpu1\")")
endif()

include(../../CompFlags.cmake)

if(${TGTNAME} STREQUAL cpu1)
  # Find ZeroMQ via pkg-config
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
endif()

include_directories("./")
include_directories("../fsw/platform_inc")
include_directories("../fsw/src")
include_directories("../shared")
include_directories("../../../fsw/apps/hwlib/fsw/public_inc")

set(demo_checkout_src
  demo_checkout.c 
  ../shared/demo_device.c
)

# Hardware library switch based on TGTNAME
if(${TGTNAME} STREQUAL cpu1)
  include_directories("${SIMULITH_DIR}/include")
  include_directories("../../../fsw/apps/hwlib/sim/inc")
  set(demo_checkout_src 
    ${demo_checkout_src}
    ${SIMULITH_DIR}/src/simulith_uart.c
    ${SIMULITH_DIR}/src/simulith_common.c
    ../../../../fsw/apps/hwlib/sim/src/libuart.c
  )
  set(demo_checkout_libs
    ${ZeroMQ_LIBRARIES}
    simulith
  )
endif()
if(${TGTNAME} STREQUAL cpu2)
  set(demo_checkout_src 
    ${demo_checkout_src}
    ../../../fsw/apps/hwlib/fsw/linux/libuart.c
  )
endif()

add_executable(demo_checkout ${demo_checkout_src})
target_link_libraries(demo_checkout ${demo_checkout_libs})

if(${TGTNAME} STREQUAL cpu1)
  set_target_properties(demo_checkout PROPERTIES COMPILE_FLAGS "-g" LINK_FLAGS "-g")
endif()
