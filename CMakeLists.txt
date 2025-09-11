cmake_minimum_required(VERSION 3.16)

option(USE_NATIVE "Optimize for host CPU (non-portable). Example Usage: -DUSE_NATIVE=ON" OFF)
set(MARCH "" CACHE STRING "Custom -march (e.g. x86-64-v2, x86-64-v3, x86-64-v4, native) - defaults to x86-64-v3.  Example Usage: -DMARCH=x86-64-v4")
set(_DEFAULT_MARCH "x86-64-v3")

project(learning-opengl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SDL_TEST OFF CACHE BOOL "" FORCE)   # don’t build SDL’s own tests
set(SDL_EXAMPLES OFF CACHE BOOL "" FORCE)  # don’t build example programs
set(SDL_INSTALL OFF CACHE BOOL "" FORCE)   # don’t install system-wide

add_subdirectory(external/SDL EXCLUDE_FROM_ALL)
add_subdirectory(external/glm EXCLUDE_FROM_ALL)

add_library(glad external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

add_library(stb_image external/stb_image/src/stb_image.c)
target_include_directories(stb_image PUBLIC external/stb_image/include)

set(SOURCES
    src/renderer.cpp
    src/shader.cpp
    src/camera.cpp
    src/cube.cpp
    src/texture.cpp
)

add_executable(learning-opengl
    src/main.cpp
    ${SOURCES}
)

target_include_directories(learning-opengl PRIVATE "${CMAKE_SOURCE_DIR}/include")

target_link_libraries(learning-opengl PRIVATE glad stb_image SDL3::SDL3 glm)

if (WIN32)
  target_link_libraries(learning-opengl PRIVATE opengl32)
elseif(APPLE)
  target_link_libraries(learning-opengl PRIVATE "-framework OpenGL")
else()
  target_link_libraries(learning-opengl PRIVATE GL)
endif()

# Set the output directory for the executable
set_target_properties(learning-opengl PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    OUTPUT_NAME "learning-opengl"
)

add_custom_command(TARGET learning-opengl POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_SOURCE_DIR}/assets
          $<TARGET_FILE_DIR:learning-opengl>/assets)

# ---------- Optimisations ----------

# Caching for improved recompliation speed 
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
  set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
  set(CMAKE_C_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif()

# Build parallelism - use 80% of available logical CPU cores
if(NOT DEFINED CMAKE_BUILD_PARALLEL_LEVEL AND NOT DEFINED ENV{CMAKE_BUILD_PARALLEL_LEVEL})
    include(ProcessorCount)
    ProcessorCount(_N)
    if(_N EQUAL 0)
        set(_N 1)
    endif()

    # Use ~80% of logical CPUs, at least 1.
    # (integer math: floor(0.8*N) == (N*4)/5)
    math(EXPR _PAR "(${_N}*4)/5")
    if(_PAR LESS 1)
        set(_PAR 1)
    endif()

    set(CMAKE_BUILD_PARALLEL_LEVEL "${_PAR}")
endif()

# Set the target machine architecture for release builds
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  # Priority: explicit override > native opt-in > default
  if(MARCH)
    set(_MARCH_FLAG "-march=${MARCH}")
  elseif(USE_NATIVE)
    set(_MARCH_FLAG "-march=native")
  else()
    set(_MARCH_FLAG "-march=${_DEFAULT_MARCH}")
  endif()

  target_compile_options(learning-opengl PRIVATE
    $<$<CONFIG:Release>:-O2>
    $<$<CONFIG:Release>:${_MARCH_FLAG}>
  )
endif()

# Enable link time optimization for Release builds
include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT ipo_error)
if(ipo_supported)
  set_property(TARGET learning-opengl PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
endif()