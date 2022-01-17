#include <iostream>
#include <Kokkos_Core.hpp>
#include "kp_all.hpp"
#include "kernels.hpp"

//-------------------------------------------------------------------------------------//

int main(int argc, char *argv[])
{
#ifdef KOKKOSTOOLS_HAS_CALIPER
  auto eventSet = KokkosTools::activate_tool("caliper", "runtime-report(profile.kokkos)");

  // Note: callbacks must be set before Kokkos::initialize()
  Kokkos::Tools::Experimental::set_callbacks(eventSet);
#endif

  Kokkos::initialize(argc, argv);

  Kokkos::DefaultExecutionSpace::print_configuration(std::cout);
  std::cout << std::endl;
  int ret_code = run_calculation(100000);
  std::cout << std::endl;

  Kokkos::finalize();
  return ret_code;
}

//-------------------------------------------------------------------------------------//
