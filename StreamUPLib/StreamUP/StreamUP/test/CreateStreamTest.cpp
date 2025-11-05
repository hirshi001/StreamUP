// test_example.cpp
#define BOOST_TEST_MODULE CreateStreamModule

#include <boost/test/included/unit_test.hpp> // header-only version

#include "StreamUP/StreamUP.h"

using namespace SUP;

BOOST_AUTO_TEST_CASE(CreateStreamTest)
{
    StreamUPConfig config;
    StreamUP<int, std::string> network(config);

    network.createStream<Reliability::UNRELIABLE_LATEST_MESSAGES_RELIABLE>(1, 10);
    network.createStream<Reliability::UNRELIABLE_ORDERED>(1);
    network.createStream<Reliability::NUM_RELIABILITY_MODES>(1);
}
