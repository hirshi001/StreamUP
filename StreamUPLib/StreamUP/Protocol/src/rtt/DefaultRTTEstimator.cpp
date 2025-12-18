#include "Protocol/rtt/DefaultRTTEstimator.h"

#include <cstdlib>

namespace SUP
{
DefaultRTTEstimator::DefaultRTTEstimator(const double initialRtt, double alpha, double initialVariance,
                                         double beta) : rtt(initialRtt), alpha(alpha), variance(initialVariance),
                                                        beta(beta)
{
}

DefaultRTTEstimator::~DefaultRTTEstimator() = default;

void DefaultRTTEstimator::addSampleImpl(double sampleRtt)
{
    const double diff = std::abs(sampleRtt - rtt);
    variance = (1 - beta) * variance + beta * diff;
    rtt = (1 - alpha) * alpha + alpha * sampleRtt;
}

}
