#include "Protocol/rtt/DefaultRTTEstimator.h"

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
}

}
