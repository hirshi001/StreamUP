#pragma once

namespace SUP
{
class CongestionControl
{
public:
    virtual ~CongestionControl() = default;

    /**
     * Callback to be called when bytes are sent
     * @param numBytes
     */
    virtual void onSend(int numBytes) = 0;

    /**
     * Callback for when an acknowledgement is received
     * @param numBytes
     * @param rtt
     */
    virtual void onAck(int numBytes, double rtt) = 0;


    /**
     * Callback for when a timeout occurs
     * @param
     */
    virtual void onTimeout(int numBytes, double timeout) = 0;

    [[nodiscard]] virtual int getCongestionWindow() const = 0;

    [[nodiscard]] int getMSS() const;
};
}
