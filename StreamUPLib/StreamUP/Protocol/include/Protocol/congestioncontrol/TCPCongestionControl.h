#pragma once

#include "Protocol/congestioncontrol/CongestionControl.h"

#include <limits>


namespace SUP
{
namespace detail
{
enum class TahoeStage
{
    SlowStart,
    CongestionAvoidance
};
}

class TCPCongestionControl : public CongestionControl
{
    static constexpr int initCwnd = 1200;

public:
    TCPCongestionControl()
    {
        cWnd = initCwnd;
        ssThresh = std::numeric_limits<int>::max();
        stage = detail::TahoeStage::SlowStart;
    }

    ~TCPCongestionControl() override = default;

    void onSend(int numBytes) override {}

    void onAck(int numBytes, double rtt) override
    {
        switch (stage)
        {
            case detail::TahoeStage::SlowStart:
                cWnd += numBytes;
                if (cWnd >= ssThresh) { stage = detail::TahoeStage::CongestionAvoidance; }
                break;
            case detail::TahoeStage::CongestionAvoidance:
                cWnd += (getMSS() * numBytes / cWnd);
                break;
        }
    }

    void onTimeout(int numBytes, double timeout) override
    {
        ssThresh = cWnd / 2;
        cWnd = initCwnd;
        stage = detail::TahoeStage::SlowStart;
    }

    [[nodiscard]] int getCongestionWindow() const override { return cWnd; }

private:
    int cWnd;
    int ssThresh; // TODO: Get initial value
    detail::TahoeStage stage;
};
}
