#---------------------------------------------------------------------------------------------------
# CMake Build Configuration for the Ray Tracing Weekend Series
#
# See README.md for guidance.
#---------------------------------------------------------------------------------------------------

cmake_minimum_required ( VERSION 3.1.0...4.0.0 )

project ( RTWeekend LANGUAGES CXX )

# Set to C++20
set ( CMAKE_CXX_STANDARD          20 )
set ( CMAKE_CXX_STANDARD_REQUIRED ON )
set ( CMAKE_CXX_EXTENSIONS        OFF )

# Source

set ( SOURCE_RT2
  src/stacy/ray-tracing2.cc
  src/stacy/colour.h
  src/stacy/vec3.h
)

set ( UTILS 
  src/stacy/common.h
  src/stacy/colour.h
  src/stacy/hit_record.h
  src/stacy/ray.h
  src/stacy/vec3.h
  # TODO make a SHAPES set
  src/stacy/sphere.h
  # TODO make a MATERIALS set
  src/stacy/material.h
)

set ( PROJ_MATRIX_CAMERA
  src/stacy/camera.h
)


include_directories(src)

# Specific compiler flags below. We're not going to add options for all possible compilers, but if
# you're new to CMake (like we are), the following may be a helpful example if you're using a
# different compiler or want to set different compiler options.

message (STATUS "Compiler ID: " ${CMAKE_CXX_COMPILER_ID})
message (STATUS "Release flags: " ${CMAKE_CXX_FLAGS_RELEASE})
message (STATUS "Debug flags: " ${CMAKE_CXX_FLAGS_DEBUG})

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # /wd #### - Disable warning
    # /we #### - treat warning as error
    add_compile_options("/W4")      # Enable level-4 warnings
    add_compile_options("/we 4265") # Class has virtual functions, but its non-trivial destructor is not virtual
    add_compile_options("/we 5204") # Class has virtual functions, but its trivial destructor is not virtual
    add_compile_options("/wd 4100") # unreferenced formal parameter
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wnon-virtual-dtor) # Class has virtual functions, but its destructor is not virtual
    add_compile_options(-Wreorder) # Data member will be initialized after [other] data member
    add_compile_options(-Wmaybe-uninitialized) # Variable improperly initialized
    add_compile_options(-Wunused-variable) # Variable is defined but unused
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wnon-virtual-dtor) # Class has virtual functions, but its destructor is not virtual
    add_compile_options(-Wreorder) # Data member will be initialized after [other] data member
    add_compile_options(-Wsometimes-uninitialized) # Variable improperly initialized
    add_compile_options(-Wunused-variable) # Variable is defined but unused
endif()

message("Compiler Version: ${CMAKE_CXX_COMPILER_VERSION}")

# Executables
add_executable(rt1             src/stacy/ray-tracing1.cc)
add_executable(rt2             ${SOURCE_RT2})
add_executable(rt3             ${UTILS} src/stacy/ray-tracing3.cc)
add_executable(rt4             ${UTILS} src/stacy/ray-tracing4.cc)
add_executable(rt4dbg          ${UTILS} src/stacy/ray-tracing4-debug.cc)

# Let's get serious
add_executable(rt5             ${UTILS} src/stacy/ray-tracing5.cc)
add_executable(rt5dbg          ${UTILS} src/stacy/ray-tracing5-debug.cc)

add_executable(rt6             ${UTILS} ${PROJ_MATRIX_CAMERA} src/stacy/ray-tracing6.cc)
add_executable(rt7             ${UTILS} ${PROJ_MATRIX_CAMERA} src/stacy/ray-tracing7.cc)

# Materials implemented
add_executable(rt8             ${UTILS} src/stacy/ray-tracing8.cc)
add_executable(rt9             ${UTILS} src/stacy/ray-tracing9.cc)
add_executable(rt10            ${UTILS} src/stacy/ray-tracing10.cc)