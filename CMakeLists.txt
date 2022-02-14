cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(KokkosWithToolsTest CXX)

include(cmake/utils.cmake)

#------------------------------------------------------------------------#
# Paths #

set(Kokkos_SRC_DIR      ${CMAKE_CURRENT_SOURCE_DIR}/kokkos)
set(KokkosTools_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/kokkos-tools)

set(Kokkos_ROOT "$ENV{Kokkos_ROOT}" CACHE FILEPATH "Path to installed Kokkos")
set(VTune_ROOT  "$ENV{VTUNE_PROFILER_2022_DIR}/sdk" CACHE FILEPATH "Path to installed Intel VTune Profiler")

#set(VARIORUM_ROOT ${KokkosTools_ROOT}/tpls/variorum/install)
#file(REAL_PATH ${VARIORUM_ROOT} VARIORUM_ROOT)

#------------------------------------------------------------------------#
# Settings #

# Kokkos
set(BACKEND_HOST) # OpenMP Theads
set(BACKEND_DEVICE) # CUDA HIP

# Tools
set(KokkosTools_ENABLE_SINGLE  ON)
set(KokkosTools_ENABLE_CALIPER ON)
set(KokkosTools_ENABLE_APEX    OFF)
set(KokkosTools_ENABLE_PAPI    OFF)

# Auto-detect MPI
find_package(MPI QUIET)
set(KokkosTools_ENABLE_MPI     ${MPI_FOUND})

#------------------------------------------------------------------------#

set(CMAKE_CXX_STANDARD 17) # Kokkos allows: 14 (default) | 17 | 20
include(cmake/ConfigureKokkos.cmake)

# Config summary
message(STATUS "======================================")
message(STATUS "Settings summary")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Generator: ${CMAKE_GENERATOR}")
message(STATUS "  Kokkos: Host=${BACKEND_HOST}, Device=${BACKEND_DEVICE}")
message(STATUS)

# Add Kokkos & Tools
if(Kokkos_ROOT)
  try_find_kokkos(${Kokkos_ROOT})
  if(Kokkos_FOUND)
    message(STATUS "Found installed Kokkos at ${Kokkos_INSTALL_DIR}")
  else()
    message(WARNING "Kokkos not found (Kokkos_ROOT=${Kokkos_ROOT})")
  endif()
else()
  message(STATUS "Hint: Point Kokkos installation with Kokkos_ROOT")
endif()
if(NOT Kokkos_FOUND)
  message(STATUS "=== Kokkos ======================================")
  set_kokkos_options()
  add_subdirectory(${Kokkos_SRC_DIR} ${CMAKE_CURRENT_BINARY_DIR}/kokkos)
endif()
message(STATUS "=== Tools =======================================")
add_subdirectory(${KokkosTools_SRC_DIR} ${CMAKE_CURRENT_BINARY_DIR}/kokkos-tools)

#------------------------------------------------------------------------#

message(STATUS "=== Test ========================================")

set(TOOLS_PATH ${CMAKE_CURRENT_BINARY_DIR}/kokkos-tools/profiling)

enable_testing()

set(TEST_APP profiling_sample_app)
add_executable(${TEST_APP} src/main.cpp)
target_link_libraries(${TEST_APP} PRIVATE Kokkos::kokkos kokkostools)

if(KokkosTools_ENABLE_MPI)
  find_package(MPI REQUIRED)
  target_link_libraries(profiling_sample_app PRIVATE MPI::MPI_CXX)
endif()

add_test(NAME test_kokkos_profiling COMMAND profiling_sample_app)
  # export KOKKOS_PROFILE_LIBRARY=${TOOLS_PATH}/simple-kernel-timer-json/libkp_kernel_timer_json.so \
  # && echo KOKKOS_PROFILE_LIBRARY=$KOKKOS_PROFILE_LIBRARY)

message(STATUS "=== Done ========================================")
