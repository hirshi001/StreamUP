// test_example.cpp
#define BOOST_TEST_MODULE BufferTestModule

#include <random>
#include <boost/test/included/unit_test.hpp> // header-only version

#include "Data/buffer/ArrayBuffer.h"


using namespace SUP;
using namespace SUP::BufferUtil;

BOOST_AUTO_TEST_CASE(BufferUIntTest)
{
    std::vector<uint8_t> array;
    array.resize(1024);

    uint8_t a = 123;
    uint16_t b = 4567;
    uint32_t c = 123456789;
    uint64_t d = 123456789123456789;
    uint8_t e = 111;

    uint8_t *data = array.data();
    writeInt<uint8_t>(data, a);
    data += sizeof(uint8_t);
    writeInt<uint16_t>(data, b);
    data += sizeof(uint16_t);
    writeInt<uint32_t>(data, c);
    data += sizeof(uint32_t);
    writeInt<uint64_t>(data, d);
    data += sizeof(uint64_t);
    writeInt<uint8_t>(data, e);
    data += sizeof(uint8_t);

    data = array.data();
    BOOST_REQUIRE_EQUAL(readInt<uint8_t>(data), a);
    data += sizeof(uint8_t);
    BOOST_REQUIRE_EQUAL(readInt<uint16_t>(data), b);
    data += sizeof(uint16_t);
    BOOST_REQUIRE_EQUAL(readInt<uint32_t>(data), c);
    data += sizeof(uint32_t);
    BOOST_REQUIRE_EQUAL(readInt<uint64_t>(data), d);
    data += sizeof(uint64_t);
    BOOST_REQUIRE_EQUAL(readInt<uint8_t>(data), e);
    data += sizeof(uint8_t);
}

BOOST_AUTO_TEST_CASE(BufferIntTest)
{
    std::vector<uint8_t> array;
    array.resize(1024);

    int8_t a = -123;
    int16_t b = 4567;
    int32_t c = -123456789;
    int64_t d = 123456789123456789;
    int8_t e = -111;

    uint8_t *data = array.data();
    writeInt<int8_t>(data, a);
    data += sizeof(int8_t);
    writeInt<int16_t>(data, b);
    data += sizeof(int16_t);
    writeInt<int32_t>(data, c);
    data += sizeof(int32_t);
    writeInt<int64_t>(data, d);
    data += sizeof(int64_t);
    writeInt<int8_t>(data, e);
    data += sizeof(int8_t);

    data = array.data();
    BOOST_REQUIRE_EQUAL(readInt<int8_t>(data), a);
    data += sizeof(int8_t);
    BOOST_REQUIRE_EQUAL(readInt<int16_t>(data), b);
    data += sizeof(int16_t);
    BOOST_REQUIRE_EQUAL(readInt<int32_t>(data), c);
    data += sizeof(int32_t);
    BOOST_REQUIRE_EQUAL(readInt<int64_t>(data), d);
    data += sizeof(int64_t);
    BOOST_REQUIRE_EQUAL(readInt<int8_t>(data), e);
    data += sizeof(int8_t);
}


BOOST_AUTO_TEST_CASE(BufferUpdateTest)
{
    std::vector<uint8_t> array;
    array.resize(1024);

    uint8_t a = 123;
    uint16_t b = 4567;
    uint32_t c = 123456789;
    uint64_t d = 123456789123456789;
    uint8_t e = 111;

    uint8_t *data = array.data();
    writeIntUpdate<uint8_t>(data, a);
    writeIntUpdate<uint16_t>(data, b);
    writeIntUpdate<uint32_t>(data, c);
    writeIntUpdate<uint64_t>(data, d);
    writeIntUpdate<uint8_t>(data, e);

    data = array.data();
    BOOST_REQUIRE_EQUAL(readInt<uint8_t>(data), a);
    data += sizeof(uint8_t);
    BOOST_REQUIRE_EQUAL(readInt<uint16_t>(data), b);
    data += sizeof(uint16_t);
    BOOST_REQUIRE_EQUAL(readInt<uint32_t>(data), c);
    data += sizeof(uint32_t);
    BOOST_REQUIRE_EQUAL(readInt<uint64_t>(data), d);
    data += sizeof(uint64_t);
    BOOST_REQUIRE_EQUAL(readInt<uint8_t>(data), e);
    data += sizeof(uint8_t);

    const uint8_t* rptr = array.data();
    BOOST_REQUIRE_EQUAL(readIntUpdate<uint8_t>(rptr), a);
    BOOST_REQUIRE_EQUAL(readIntUpdate<uint16_t>(rptr), b);
    BOOST_REQUIRE_EQUAL(readIntUpdate<uint32_t>(rptr), c);
    BOOST_REQUIRE_EQUAL(readIntUpdate<uint64_t>(rptr), d);
    BOOST_REQUIRE_EQUAL(readIntUpdate<uint8_t>(rptr), e);
}


BOOST_AUTO_TEST_CASE(BufferAllTypesTest)
{
    std::vector<uint8_t> array;
    array.resize(1024);

    uint8_t a = 123;
    uint16_t b = 4567;
    uint32_t c = 123456789;
    uint64_t d = 123456789123456789;
    int8_t e = -123;
    int16_t f = 4567;
    int32_t g = -123456789;
    int64_t h = 123456789123456789;
    float i = 127.43F;
    double j = 1356245138426594.12452364;



    uint8_t *data = array.data();
    writeUpdate(data, a);
    writeUpdate(data, b);
    writeUpdate(data, c);
    writeUpdate(data, d);
    writeUpdate(data, e);
    writeUpdate(data, f);
    writeUpdate(data, g);
    writeUpdate(data, h);
    writeUpdate(data, i);
    writeUpdate(data, j);

    const uint8_t* rptr = array.data();
    BOOST_REQUIRE_EQUAL(readUpdate<uint8_t>(rptr), a);
    BOOST_REQUIRE_EQUAL(readUpdate<uint16_t>(rptr), b);
    BOOST_REQUIRE_EQUAL(readUpdate<uint32_t>(rptr), c);
    BOOST_REQUIRE_EQUAL(readUpdate<uint64_t>(rptr), d);
    BOOST_REQUIRE_EQUAL(readUpdate<int8_t>(rptr), e);
    BOOST_REQUIRE_EQUAL(readUpdate<int16_t>(rptr), f);
    BOOST_REQUIRE_EQUAL(readUpdate<int32_t>(rptr), g);
    BOOST_REQUIRE_EQUAL(readUpdate<int64_t>(rptr), h);
    BOOST_REQUIRE_EQUAL(readUpdate<float>(rptr), i);
    BOOST_REQUIRE_EQUAL(readUpdate<double>(rptr), j);
}
