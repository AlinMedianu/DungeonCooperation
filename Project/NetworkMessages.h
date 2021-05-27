#pragma once

#include <concepts>
#include <SFML/Config.hpp>
#include <SFML/System/Vector2.hpp>

namespace Network
{
    template<typename T>
    concept Message = requires(const T& message)
	{
		{ message.Size() } noexcept -> std::same_as<sf::Uint8>;
	};

	enum class HandshakeMessageID : sf::Uint8
	{
		Synchronize, SynchronizeAcknowledgement, COUNT
	};

	enum class HandshakeProperties : sf::Uint8
	{
		None, 
		Acknowledged = 1 << 0, 
		Rejected = 1 << 1,
		Versions = 1 << 2,
		All = 0x07
	};

	enum class MessageProperties : sf::Uint8
	{
		None,
		Reliable = 1 << 0,
		ToThis = 1 << 1,
		ToOther = 1 << 2,
		Acknowledged = 1 << 3,
		Rejected = 1 << 4,
		ClientTimestamp = 1 << 5,
		Versions = 1 << 6,
		All = 0x3f
	};

	enum class TagID : sf::Uint8
	{
		Disconnect,
		COUNT
	};

	enum class MessageID : sf::Uint8
	{
		Synchronize, SynchronizeAcknowledgement, FirstTag, 
		Seed = FirstTag + static_cast<sf::Uint8>(TagID::COUNT),
		Position,
		COUNT
	};

	struct Synchronize
	{
		sf::Uint16 synchronize;
		[[nodiscard]] constexpr sf::Uint8 Size() const noexcept
		{
			return sizeof(synchronize);
		}
	};

	struct SynchronizeAcknowledgement
	{
		sf::Uint16 synchronize;
		sf::Uint16 acknowledgement;
		[[nodiscard]] constexpr sf::Uint8 Size() const noexcept
		{
			return sizeof(synchronize) + sizeof(acknowledgement);
		}
	};

	struct KeepAlive
	{

	};

	struct Tag
	{
		TagID id;
		[[nodiscard]] constexpr sf::Uint8 Size() const noexcept
		{
			return 0;
		}
	};

	struct Seed
	{
		int seed;
		[[nodiscard]] constexpr sf::Uint8 Size() const noexcept
		{
			return sizeof(seed);
		}
	};

	struct Position
	{
		sf::Vector2f position;
		[[nodiscard]] constexpr sf::Uint8 Size() const noexcept
		{
			return sizeof(position);
		}
	};

}
