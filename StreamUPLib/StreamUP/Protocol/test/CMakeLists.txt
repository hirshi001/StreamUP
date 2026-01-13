add_executable(StreamUPProtocolTests
        TestSetup.cpp
        TCPTest.cpp
        SecurityTypesTest.cpp
        ProtocolUsageExample.cpp
)
target_link_libraries(StreamUPProtocolTests PRIVATE Boost::unit_test_framework StreamUPProtocol asio)

add_test(NAME StreamUPProtocolTestsBoostTests COMMAND StreamUPProtocolTests)
