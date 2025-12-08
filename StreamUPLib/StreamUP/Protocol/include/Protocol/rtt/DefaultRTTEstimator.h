#pragma once
#include <limits>

#include "RTTEstimator.h"

namespace SUP
{
class DefaultRTTEstimator : public RTTEstimator<DefaultRTTEstimator>
{

public:
    explicit DefaultRTTEstimator(double initialRtt = 2, double alpha = 0.125, double initialVariance = 1, double beta = 0.25);

    ~DefaultRTTEstimator() override;

private:
    void addSampleImpl(double sampleRtt);

    double rtt;
    double alpha;

    double variance;
    double beta;
};
}
