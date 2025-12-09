add_executable(StreamUPProtocolTests
        TestSetup.cpp
        TCPTest.cpp
        SecurityTypesTest.cpp
)
target_link_libraries(StreamUPProtocolTests PRIVATE Boost::unit_test_framework StreamUPProtocol)

add_test(NAME StreamUPProtocolTestsBoostTests COMMAND StreamUPProtocolTests)
