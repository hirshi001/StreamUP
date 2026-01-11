#pragma once

namespace SUP
{
enum class StreamReliability
{
    RELIABLE_SEQUENCED,
    RELIABLE_UNSEQUENCED,
    UNRELIABLE_SEQUENCED,
    UNRELIABLE_UNSEQUENCED,
    LATEST_MESSAGE_RELIABILITY
};

enum class StreamStarvability
{
    UNSTARVABLE,
    STARVABLE
};

class Stream
{
public:
    explicit Stream(StreamReliability reliability, StreamStarvability starvability,
                    int priorityWeight) : reliability(reliability), starvability(starvability),
                                             priorityWeight(priorityWeight) {}

private:
    StreamReliability reliability;
    StreamStarvability starvability;

    int priorityWeight;
    int deficitCounter{};
};
}
