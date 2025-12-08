// test_example.cpp
#define BOOST_TEST_MODULE BufferTestModule

#include <random>
#include <boost/test/included/unit_test.hpp> // header-only version

#include "Protocol/buffer/ArrayBuffer.h"


using namespace SUP;
using namespace SUP::BufferUtil;

BOOST_AUTO_TEST_CASE(BufferUtilTest)
{
    std::vector<uint8_t> array;
    array.reserve(1024);

    writeInt<uint8_t>(array.data(), 1);
    writeInt<uint8_t>(array.data() + 1, 2);
    writeInt<uint8_t>(array.data() + 2, 3);
    writeInt<uint8_t>(array.data() + 3, 4);

    assert(readInt<uint8_t>(array.data()) == 1);
    assert(readInt<uint8_t>(array.data() + 1) == 2);
    assert(readInt<uint8_t>(array.data() + 2) == 3);
    assert(readInt<uint8_t>(array.data() + 3) == 4);
}
