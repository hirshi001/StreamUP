// test_example.cpp
#define BOOST_TEST_MODULE TCPTestModule

#include <random>
#include <boost/test/included/unit_test.hpp> // header-only version

#include "Protocol/congestioncontrol/TCPCongestionControl.h"
#include "Protocol/rtt/DefaultRTTEstimator.h"
#include "Protocol/rtt/RTTEstimator.h"


using namespace SUP;

enum class EventType
{
    EXPECT_ACK,
    RTO
};

struct PacketEvent
{
    double time;
    EventType type;
    int packetId;
};

struct CompareAckRTO
{
    bool operator()(const PacketEvent& a, const PacketEvent& b) const
    {
        return a.time > b.time;
    }
};

BOOST_AUTO_TEST_CASE(TCPTestExample)
{
    /*
    std::priority_queue<PacketEvent, std::vector<PacketEvent>, CompareAckRTO> events;
    TCPCongestionControl cc;

    int bytesInFlight = 0;
    constexpr int totalBytesToSend = 1000000;
    constexpr int maxPacketSize = 1200;
    int bytesToSend = totalBytesToSend;
    int bytesAcked = 0;
    double rtt = 100; // ms

    DefaultRTTEstimator rttEstimator(100, 0.125, 1, 0.25);

    std::default_random_engine rng;
    std::bernoulli_distribution drop(0.05); // 10% packet loss

    double now = 0;
    while (true)
    {
        // sending logic
        while (cc.getCongestionWindow() > 0)
        {
            int bytesToSendNow = cc.getCongestionWindow() - bytesInFlight;
            bytesToSendNow = std::min(bytesToSendNow, bytesToSend);
            bytesToSendNow = std::min(bytesToSendNow, maxPacketSize);
            ackTimes.emplace(now + rtt, now + rttEstimator.getRTO(), bytesToSendNow);
        }

        // ack/rto logic
        double nextAckTime = 0;
        double nextRTOTime = 0;
        if (!ackTimes.empty())
        {
            SentPacket nextAck = ackTimes.top();
            nextAckTime = ackTimes.top().rto;
        }
        if (nextAckTime > nextRTOTime)
        {

        }




        if (drop(rng))
        {
            // add the sentPacket to rto min heap
            cc.onTimeout(bytesToSend, rtt);
            std::cout << "Timeout! cwnd = " << cc.getCongestionWindow() << "\n";
        }


        // Random drop
         else
        {
            cc.onAck(bytesToSend, rtt);
            rttEstimator.addSample(rtt);
            std::cout << "ACK received! cwnd = " << cc.getCongestionWindow() << "\n";
        }

        // wait a bit to simulate RTT
        // std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    std::cout << "Total bytes sent: " << totalBytesSent << "\n";
    */
}
