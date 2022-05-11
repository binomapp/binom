#ifndef BITS_TEST_H
#define BITS_TEST_H

#include "tester.hxx"
#include "libbinom/include/variables/bit_array.hxx"
#include <assert.h>
#include <array>

void testBits() {
  RAIIPerfomanceTest test_perf("Bits test: ");
  using namespace binom::priv;
  TEST_ANNOUNCE(Test Bits & Bits value);
  GRP(
    PRINT_RUN(Bits bits;);
    PRINT_RUN(Bits::Value bit_first = bits[0];);
    PRINT_RUN(Bits::Value bit_last = bits[7];);

    LOG("bit_first: " << bit_first << "; bit_last: " << bit_last);
    LOG("bit_first == bit_last: " << (bit_first == bit_last));
    PRINT_RUN(bit_first = true;);
    LOG("bit_first: " << bit_first << "; bit_last: " << bit_last);
    LOG("bit_first == bit_last: " << (bit_first == bit_last));
    PRINT_RUN(bit_last = bit_first;);
    LOG("bit_first: " << bit_first << "; bit_last: " << bit_last);
    LOG("bit_first == bit_last: " << (bit_first == bit_last));
  );
//  std::pair<size_t, size_t> test_cases[] = {
//    {7,12},
//    {2,5},
//    {9,20}
//  };
//  TEST_ANNOUNCE(Test BitArrayHeader);
//  GRP(
//    PRINT_RUN(BitArrayHeader* data_ptr = BitArrayHeader::create({false, true, true, false, true, false, false, true, true, true, true}));
//    TEST_ANNOUNCE(Test BitArrayHeader::insertBits)
//    GRP(
//      for(auto element : test_cases) {
//        const size_t at = element.first;
//        const size_t count = element.second;
//        LOG("Test case: size_t at = " << at << "; size_t count = " << count << ";")
//        {
//          LOG("Print bits:")
//          int counter = 0;
//          for(const auto &element : *data_ptr)
//            LOG( counter++ << " - " << element);
//          LOG("==================");
//        }
//        PRINT_RUN(BitArrayHeader* copy_ptr = BitArrayHeader::copy(data_ptr););
//        PRINT_RUN(BitArrayHeader::insertBits(data_ptr, at, count););
//        PRINT_RUN(BitArrayHeader& data = *data_ptr;);
//        PRINT_RUN(BitArrayHeader& copy = *copy_ptr;);

//        {
//          LOG("Print bits:")
//          int counter = 0;
//          for(const auto &element : *data_ptr)
//            LOG( counter++ << " - " << element);
//          LOG("==================");
//        }


//        LOG("Check old segments of data:");
//        for(size_t i = 0; i < at; ++i) {
//          LOG("Index:" << i);
//          TEST(data[i] == copy[i]);
//        }

//        for(size_t i = at; i < copy.getBitSize(); ++i) {
//          LOG("Old index:" << i << "; New index: " << (i + count));
//          TEST(data[i + count] == copy[i]);
//        }

//        PRINT_RUN(delete copy_ptr;);
//      }
//    )
//    PRINT_RUN(delete data_ptr;);
//  )
}

#endif // BITS_TEST_H
