#pragma once

#include <chrono>
#include <SFML/Config.hpp>

namespace Network
{
	using seconds = std::chrono::duration<float>;
	using milliseconds = std::chrono::milliseconds;
	class RetransmissionTimeoutCalculator
	{
		const float averageGain;
		const float deviationGain;
		seconds smoothedTime;
		seconds smoothedMeanDeviation;
		seconds timeout;
	public:
		RetransmissionTimeoutCalculator() noexcept;
		void UpdateFrom(milliseconds roundTripTime) noexcept;
		[[nodiscard]] milliseconds GetTimeout(sf::Uint8 numberOfFailedRetransmissions) const noexcept;
	};
}