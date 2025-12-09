// test_example.cpp
#define BOOST_TEST_MODULE BufferTestModule

#include <random>
#include <boost/test/included/unit_test.hpp> // header-only version

#include "Data/buffer/BufferUtil.h"


using namespace SUP;
using namespace SUP::BufferUtil;

BOOST_AUTO_TEST_CASE(BufferTest)
{
    std::vector<uint8_t> array;
    array.resize(1024);

    uint8_t wA1 = 123, rA1;
    uint16_t wB1 = 4567, rB1;
    uint32_t wC1 = 123456789, rC1;
    uint64_t wD1 = 123456789123456789, rD1;
    uint8_t wE1 = 111, rE1;


    int8_t wA2 = -123, rA2;
    int16_t wB2 = 4567, rB2;
    int32_t wC2 = -123456789, rC2;
    int64_t wD2 = 123456789123456789, rD2;
    int8_t wE2 = 111, rE2;

    float wA3 = 135.531513, rA3;
    double wB3 = 5398647.421, rB3;


    uint8_t *data = array.data();
    data += write<uint8_t>(data, wA1);
    data += write<uint16_t>(data, wB1);
    data += write<uint32_t>(data, wC1);
    data += write<uint64_t>(data, wD1);
    data += write<uint8_t>(data, wE1);

    data += write<int8_t>(data, wA2);
    data += write<int16_t>(data, wB2);
    data += write<int32_t>(data, wC2);
    data += write<int64_t>(data, wD2);
    data += write<int8_t>(data, wE2);

    data += write<float>(data, wA3);
    data += write<double>(data, wB3);


    data = array.data();
    data += read<uint8_t>(data, rA1);
    BOOST_REQUIRE_EQUAL(wA1, rA1);
    data += read<uint16_t>(data, rB1);
    BOOST_REQUIRE_EQUAL(wB1, rB1);
    data += read<uint32_t>(data, rC1);
    BOOST_REQUIRE_EQUAL(wC1, rC1);
    data += read<uint64_t>(data, rD1);
    BOOST_REQUIRE_EQUAL(wD1, rD1);
    data += read<uint8_t>(data, rE1);
    BOOST_REQUIRE_EQUAL(wE1, rE1);

    data += read<int8_t>(data, rA2);
    BOOST_REQUIRE_EQUAL(wA2, rA2);
    data += read<int16_t>(data, rB2);
    BOOST_REQUIRE_EQUAL(wB2, rB2);
    data += read<int32_t>(data, rC2);
    BOOST_REQUIRE_EQUAL(wC2, rC2);
    data += read<int64_t>(data, rD2);
    BOOST_REQUIRE_EQUAL(wD2, rD2);
    data += read<int8_t>(data, rE2);
    BOOST_REQUIRE_EQUAL(wE2, rE2);

    data += read<float>(data, rA3);
    BOOST_REQUIRE_EQUAL(wA3, rA3);
    data += read<double>(data, rB3);
    BOOST_REQUIRE_EQUAL(wB3, rB3);
}
