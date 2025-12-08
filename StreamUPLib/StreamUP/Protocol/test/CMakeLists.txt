add_executable(StreamUPProtoclTCPTests TCPTest.cpp
        BufferTest.cpp)
target_link_libraries(StreamUPProtoclTCPTests PRIVATE Boost::unit_test_framework StreamUPProtocol)

add_test(NAME StreamUPProtoclTCPBoostTests COMMAND StreamUPProtoclTCPTests)
