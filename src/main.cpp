#include <iostream>
#include <Kokkos_Core.hpp>
#include "kp_all.hpp"
#include "kernels.hpp"

#if USE_MPI
#include <mpi.h>
#endif

//-------------------------------------------------------------------------------------//

int main(int argc, char *argv[])
{
#if USE_MPI
  MPI_Init(&argc, &argv);
#endif
  const char *profiler_name = "";
  const char *profiler_config = "";
#ifdef KOKKOSTOOLS_HAS_CALIPER
  profiler_name = "caliper";
  profiler_config = "runtime-report(profile.kokkos)";
#endif
#if USE_MPI
  profiler_name = "highwater-mark-mpi";
#endif

  profiler_name = "vtune-connector";
  profiler_config = "";

  // TODO: Utilities: common/kernel-filter
  //                  debugging/kernel-logger

  auto eventSet = KokkosTools::get_event_set(profiler_name, profiler_config);

  // Note: callbacks must be set before Kokkos::initialize()
  Kokkos::Tools::Experimental::set_callbacks(eventSet);

  Kokkos::initialize(argc, argv);

  Kokkos::DefaultExecutionSpace::print_configuration(std::cout);
  std::cout << std::endl;
  int ret_code = run_calculation(100000);
  std::cout << std::endl;

  Kokkos::finalize();
#if USE_MPI
  MPI_Finalize();
#endif
  return ret_code;
}

//-------------------------------------------------------------------------------------//
