#include <iostream>
#include <Kokkos_Core.hpp>
#include "kp_all.hpp"

template <typename data_type>
extern int run_calculation(const data_type SIZE); // defined below

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

// Sample computation: S(N) = 1 + 2 + 3 + ... + N
// Tests: regions, allocation, parallel for and reduction
template <typename data_type>
int run_calculation(const data_type SIZE)
{
  Kokkos::Profiling::pushRegion("Computation");

  Kokkos::View<data_type*> data(Kokkos::ViewAllocateWithoutInitializing("data"), SIZE);
  Kokkos::parallel_for("initialize()", SIZE, KOKKOS_LAMBDA(data_type i) {
    data(i) = i;
  });
  Kokkos::fence();

  data_type sum = 0;
  Kokkos::parallel_reduce("accumulate()", SIZE, KOKKOS_LAMBDA(data_type i, data_type &lsum) {
    lsum += 1 + data(i);
  }, sum);
  Kokkos::fence();

  Kokkos::Profiling::popRegion();

  // check results
  const data_type check = (SIZE + 1) * SIZE / 2;
  if (sum != check) {
    std::cout << "BAD result, got S(" << SIZE << ") = " << sum << " (expected " << check << ")" << std::endl;
    return 1;
  }
  std::cout << "Result OK: S(" << SIZE << ") = " << sum << std::endl;
  return 0;
}
