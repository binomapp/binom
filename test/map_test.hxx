#ifndef MAP_TEST_HXX
#define MAP_TEST_HXX

#include "tester.hxx"

#include "libbinom/include/variables/map.hxx"
#include "print_variable.hxx"

void testMap() {
  using namespace binom::literals;
  using namespace binom;

  RAIIPerfomanceTest test_perf("Map test: ");
  SEPARATOR
  TEST_ANNOUNCE(Map test)
  GRP_PUSH

  LOG("(bug in [r]): binom::Map _map = map{{1, 2},{3, 4}};");
  binom::Map _map = map{{1, 2},{3, 4}};

  PRINT_RUN(_map[4] = 5;)
  PRINT_RUN(_map[KeyValue(nullptr)];)
  PRINT_RUN(_map[-1] = 0;)
  LOG("(bug in [r]): _map[6] = i32arr{7,8,9,10};");
  _map[6] = i32arr{7,8,9,10};

  utils::printVariable(_map.move());

  GRP_POP
}


#endif // MAP_TEST_HXX
