#pragma once

namespace SUP
{

template <typename T>
concept HasAddSampleImpl = requires(T& obj) {
    obj.addSampleImpl();
};

template<typename Derived>
class RTTEstimator
{
public:
    RTTEstimator() = default;
    virtual ~RTTEstimator() = default;

    void addSample(double rtt)
    {
        static_cast<Derived*>(this)->addSampleImpl(rtt);
    }

    [[nodiscard]] double getRTTEstimate() const
    {
        return rtt;
    }

    [[nodiscard]] double getRTTVariance() const
    {
        return variance;
    }

    [[nodiscard]] double getRTO() const
    {
        return 4 * getRTTVariance() + getRTTEstimate();
    }

    void setRTTEstimate(double _rtt)
    {
        rtt = _rtt;
    }

    void setRTTVariance(double _variance)
    {
        variance = _variance;
    }

protected:
    double rtt;
    double variance;
};
}