#include "NetworkRetransmissionTimeoutCalculator.h"

using namespace std::chrono_literals;

namespace Network
{
	RetransmissionTimeoutCalculator::RetransmissionTimeoutCalculator() noexcept
		: averageGain{ 0.125f }, deviationGain{ 0.25f }, smoothedTime{}, 
		smoothedMeanDeviation{}, timeout{ 3s }
	{

	}

	void RetransmissionTimeoutCalculator::UpdateFrom(milliseconds roundTripTime) noexcept
	{
		seconds errorMargin(roundTripTime - smoothedTime);
		smoothedTime += averageGain * errorMargin;
		smoothedMeanDeviation += deviationGain * (seconds(std::abs(errorMargin.count())) - smoothedMeanDeviation);
		timeout = smoothedTime + 4 * smoothedMeanDeviation;
	}

	milliseconds RetransmissionTimeoutCalculator::GetTimeout(sf::Uint8 numberOfFailedRetransmissions) const noexcept
	{
		return floor<milliseconds>(timeout * powf(2, static_cast<float>(numberOfFailedRetransmissions)));
	}
}