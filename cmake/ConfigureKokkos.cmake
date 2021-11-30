set_if_match(Kokkos_ENABLE_DEBUG CMAKE_BUILD_TYPE Debug)
set(BUILD_SHARED_LIBS ON)
set(Kokkos_CXX_STANDARD 14) # 14 (default) | 17 | 20
set(Kokkos_ENABLE_PROFILING_LOAD_PRINT ON)
set(Kokkos_ENABLE_EXAMPLES OFF)
set(Kokkos_ENABLE_TESTS OFF)
set(Kokkos_ENABLE_LIBDL ON)
set(Kokkos_ENABLE_HWLOC ON)
set(Kokkos_ENABLE_LIBRT ON)

set_if_match(Kokkos_ENABLE_OPENMP  BACKEND_HOST   OpenMP)
set_if_match(Kokkos_ENABLE_PTHREAD BACKEND_HOST   Threads)
set_if_match(Kokkos_ENABLE_CUDA    BACKEND_DEVICE CUDA)
# set(Kokkos_ENABLE_SERIAL ON) # Kokkos will enable serial if nothing else is enabled

if(Kokkos_ENABLE_CUDA)
  # set(CMAKE_CXX_COMPILER nvcc)

  set(Kokkos_ENABLE_CUDA_LAMBDA                  ${Kokkos_ENABLE_CUDA})
  set(Kokkos_ENABLE_CUDA_UVM                     ${Kokkos_ENABLE_CUDA})
  set(Kokkos_ENABLE_CUDA_CONSTEXPR               ${Kokkos_ENABLE_CUDA})
  set(Kokkos_ENABLE_CUDA_LDG_INTRINSIC           ${Kokkos_ENABLE_CUDA})
  set(Kokkos_ENABLE_CUDA_RELOCATABLE_DEVICE_CODE OFF)

  #export PATH=$PATH:$CUDA_ROOT/bin
  #export NVCC_WRAPPER_DEFAULT_COMPILER=$(which c++)
  #export CUDA_LAUNCH_BLOCKING=1
  #export CUDA_MANAGED_FORCE_DEVICE_ALLOC=1
endif()
