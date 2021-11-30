cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(KokkosWithToolsTest)

include(cmake/utils.cmake)

#------------------------------------------------------------------------#
# Paths #

set(Kokkos_ROOT      ${CMAKE_CURRENT_SOURCE_DIR}/kokkos)
set(KokkosTools_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/kokkos-tools)

#------------------------------------------------------------------------#
# Settings #

# Kokkos
set(BACKEND_HOST Threads) # OpenMP Theads
set(BACKEND_DEVICE) # CUDA HIP

# Tools
set(KokkosTools_ENABLE_SINGLE  ON)
set(KokkosTools_ENABLE_CALIPER ON)
set(KokkosTools_ENABLE_APEX    OFF)
set(KokkosTools_ENABLE_PAPI    OFF)
set(KokkosTools_ENABLE_MPI     OFF)

#------------------------------------------------------------------------#

include(cmake/ConfigureKokkos.cmake)

# Config summary
message(STATUS "======================================")
message(STATUS "Settings summary")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Generator: ${CMAKE_GENERATOR}")
message(STATUS "  Kokkos: Host=${BACKEND_HOST}, Device=${BACKEND_DEVICE}")
message(STATUS)

# Add Kokkos & Tools
message(STATUS "=== Kokkos ======================================")
add_subdirectory(${Kokkos_ROOT} ${CMAKE_CURRENT_BINARY_DIR}/kokkos)
message(STATUS "=== Tools =======================================")
add_subdirectory(${KokkosTools_ROOT} ${CMAKE_CURRENT_BINARY_DIR}/kokkos-tools)

#------------------------------------------------------------------------#

message(STATUS "=== Test ========================================")

set(TOOLS_PATH ${CMAKE_CURRENT_BINARY_DIR}/kokkos-tools/profiling)

enable_testing()

set(TEST_APP profiling_sample_app)
add_executable(${TEST_APP} src/main.cpp)
target_link_libraries(${TEST_APP} PRIVATE Kokkos::kokkos kokkostools)

add_test(NAME test_kokkos_profiling COMMAND profiling_sample_app)

message(STATUS "=== Done ========================================")
