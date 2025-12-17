#pragma once

#include <map>

#include "Protocol/packet/SendPacket.h"

namespace SUP
{
class SendPacketPool
{
public:
    SendPacketPool(SendPacketPool &) = delete;

    SendPacketPool(SendPacketPool &&) = delete;

    SendPacketPool &operator=(const SendPacketPool &) = delete;

    SendPacketPool &operator=(SendPacketPool &&) = delete;

    explicit SendPacketPool(int margin) : margin(margin)
    {
    }

    ~SendPacketPool()
    = default;

    /**
     *
     * @param bufferSize This should typically be MTU
     */
    void addClassSize(size_t bufferSize)
    {
        size_t lower = bufferSize;
        size_t upper = bufferSize + margin;

        // check if any class exists in range [lower, upper]
        auto it = sendPackets.lower_bound(lower);
        if (it != sendPackets.end() && it->first <= upper)
        {
            // A suitable class size already exists
            return;
        }

        // no existing class => add one
        sendPackets.emplace(bufferSize, std::vector<std::unique_ptr<SendPacket> >{});
    }

    std::unique_ptr<SendPacket> getSendPacket(size_t bufferSize)
    {
        // find the first class >= requested size
        auto it = sendPackets.lower_bound(bufferSize);

        if (it == sendPackets.end())
        {
            // no class big enough → create an on-demand SendPacket
            return std::make_unique<SendPacket>(bufferSize);
        }

        size_t classSize = it->first;
        auto &vec = it->second;

        if (!vec.empty())
        {
            // reuse an existing packet
            std::unique_ptr<SendPacket> pkt = std::move(vec.back());
            vec.pop_back();
            return pkt;
        }

        // no free packet → allocate new one
        return std::make_unique<SendPacket>(classSize);
    }

    void releaseSendPacket(std::unique_ptr<SendPacket> pkt)
    {
        if (!pkt)
            return;

        auto it = sendPackets.find(pkt->bufferCapacity());
        if (it == sendPackets.end())
            return;

        it->second.push_back(std::move(pkt));
    }

private:
    int margin;
    std::map<size_t, std::vector<std::unique_ptr<SendPacket> > > sendPackets;
};
}
