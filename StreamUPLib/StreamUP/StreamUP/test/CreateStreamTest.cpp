// test_example.cpp
#define BOOST_TEST_MODULE CreateStreamModule

#include <boost/test/included/unit_test.hpp> // header-only version

#include "../../Platform/include/Platform/Address.h"
#include "StreamUP/StreamUP.h"
#include "../include/StreamUP/Connection/Connection.h"

using namespace SUP;

BOOST_AUTO_TEST_CASE(CreateStreamTest)
{
    constexpr uint64_t max = std::numeric_limits<uint64_t>::max();
    const StreamUPConfig networkConfig(max, max, max, max);
    StreamUP network(networkConfig);

    const ConnectionConfig connectionConfig(0, max);
    auto [connection, result] = network.createConnection(connectionConfig);
    assert(result == CreateConnectionResult::SUCCESS);

    const Address address("localhost", 1234);
    connection->connect(address);


    network.createStream<Reliability::UNRELIABLE_ORDERED>(1);
    network.createStream<Reliability::NUM_RELIABILITY_MODES>(1);
}
