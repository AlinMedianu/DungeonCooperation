#include "NetworkPool.h"

namespace Network
{
	void Pool::Emplace(Synchronize&& message, HandshakeProperties properties, 
		const SOCKADDR& sender, std::optional<sf::Uint8> version)
	{
		syncronizations.emplace(message, properties, sender, std::nullopt, version);
	}

	void Pool::Emplace(SynchronizeAcknowledgement&& message, HandshakeProperties properties,
		const SOCKADDR& sender, std::optional<sf::Uint8> version)
	{
		syncronizeAcknowledgements.emplace(message, properties, sender, std::nullopt, version);
	}

	void Pool::Emplace(Tag&& message, MessageProperties properties,
		const SOCKADDR& sender, std::optional<float> timestamp, std::optional<sf::Uint8> version)
	{
		tags.emplace(message, properties, sender, timestamp, version);
	}

	void Pool::Emplace(Seed&& message, MessageProperties properties,
		const SOCKADDR& sender, std::optional<float> timestamp, std::optional<sf::Uint8> version)
	{
		seeds.emplace(message, properties, sender, timestamp, version);
	}

	void Pool::Emplace(Position&& message, MessageProperties properties,
		const SOCKADDR& sender, std::optional<float> timestamp, std::optional<sf::Uint8> version)
	{
		positions.emplace(message, properties, sender, timestamp, version);
	}

	void Pool::Push(const SOCKADDR& keepAliveAddress)
	{
		keepAliveAddresses.push(keepAliveAddress);
	}

	void Pool::TrapMessages(Trap& trap)
	{
		if (!syncronizations.empty() && trap.syncronization.TryWrite(syncronizations.front()))
			syncronizations.pop();
		if (!syncronizeAcknowledgements.empty() && 
			trap.syncronizeAcknowledgement.TryWrite(syncronizeAcknowledgements.front()))
			syncronizeAcknowledgements.pop();
		if (!tags.empty() && trap.tag.TryWrite(tags.front()))
			tags.pop();
		if (!seeds.empty() && trap.seed.TryWrite(seeds.front()))
			seeds.pop();
		if (!positions.empty() && trap.position.TryWrite(positions.front()))
			positions.pop();
		if (!keepAliveAddresses.empty() && trap.heartBeat.TryWrite(keepAliveAddresses.front()))
			keepAliveAddresses.pop();
	}
}