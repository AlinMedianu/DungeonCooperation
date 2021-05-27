#pragma once

#include <SFML/System/Clock.hpp>
#include "NetworkSocket.h"
#include "NetworkRetransmissionTimeoutCalculator.h"

namespace Network
{
	class Retransmitter
	{
		Socket& sender;
		const RetransmissionTimeoutCalculator& timeoutCalculator;
		Packet data;
		SOCKADDR receiver;
		std::optional<sf::Clock> timeoutTimer;
		std::optional<sf::Clock> runningTimer;
		milliseconds timeout;
		sf::Uint8 failedAttemptsCount;
		void Transmit();
	public:
		explicit Retransmitter(Socket& sender, const RetransmissionTimeoutCalculator& timeoutCalculator) noexcept;
		[[nodiscard]] bool IsRunning() const noexcept;
		[[nodiscard]] const Packet& GetPacket() const noexcept;
		[[nodiscard]] const SOCKADDR& GetReceiver() const noexcept;
		void Start(const Packet& data, const SOCKADDR& receiver);
		void Update();
		milliseconds StopAndGetTimeSinceStart();
	};
}

