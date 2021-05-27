#pragma once

#include <queue>
#include <optional>
#include "Enumeration.h"
#include "NetworkTrap.h"

namespace Network
{
    class Pool
    {
		std::queue<ExtendedMessage<Synchronize, HandshakeProperties>> syncronizations;
		std::queue<ExtendedMessage<SynchronizeAcknowledgement, HandshakeProperties>> syncronizeAcknowledgements;
		std::queue<ExtendedMessage<Tag, MessageProperties>> tags;
		std::queue<ExtendedMessage<Seed, MessageProperties>> seeds;
		std::queue<ExtendedMessage<Position, MessageProperties>> positions;
		std::queue<SOCKADDR> keepAliveAddresses;
	public:
		void Emplace(Synchronize&& message, HandshakeProperties properties, const SOCKADDR& sender,
			std::optional<sf::Uint8> version);
		void Emplace(SynchronizeAcknowledgement&& message, HandshakeProperties properties, const SOCKADDR& sender,
			std::optional<sf::Uint8> version);
		void Emplace(Tag&& message, MessageProperties properties, const SOCKADDR& sender,
			std::optional<float> timestamp, std::optional<sf::Uint8> version);
		void Emplace(Seed&& message, MessageProperties properties, const SOCKADDR& sender,
			std::optional<float> timestamp, std::optional<sf::Uint8> version);
		void Emplace(Position&& message, MessageProperties properties, const SOCKADDR& sender,
			std::optional<float> timestamp, std::optional<sf::Uint8> version);
		void Push(const SOCKADDR& keepAliveAddress);
		void TrapMessages(Trap& trap);
    };
}
