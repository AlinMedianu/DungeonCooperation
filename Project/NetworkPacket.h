#pragma once

#include <cassert>
#include <string>
#include "Math.h"
#include "NetworkPool.h"

namespace Network
{
	constexpr size_t maxPacketSize = 64;
	struct Packet
	{	
		Packet() noexcept;
		void Process(HandshakeProperties properties, HandshakeMessageID message);
		void Process(KeepAlive message) noexcept;
		void Process(MessageProperties properties, TagID message);
		void Process(MessageProperties properties, Seed message);
		void Process(MessageProperties properties, Position message);
		void Clear();
		[[nodiscard]] MessageProperties GetProperties() const;
		[[nodiscard]] std::string_view GetPayload() const noexcept;
		[[nodiscard]] std::string& GetPayload() noexcept;
		void DumpInto(Pool& pool, const SOCKADDR& sender);
		template<Math::Number Number>
		[[nodiscard]] bool TryInject(Number number, bool leaveOpen = false) noexcept;
	private:
		struct Hole
		{
			sf::Uint8 position;
			sf::Uint8 size;
		};
		std::string payload;
		std::queue<Hole> holes;
		[[nodiscard]] constexpr sf::Uint8 From(MessageProperties properties) const noexcept;
		void ReserveSpaceForOptionalData(MessageProperties properties, sf::Uint8 from);
		void GetOptionalData(std::optional<float>& timestamp, 
			std::optional<sf::Uint8>& version, sf::Uint8 optionalDataStart) const;
		void DumpHandshakeMessageInto(Message auto message, 
			Pool& pool, const SOCKADDR& sender);
		void DumpMessageInto(Message auto message, Pool& pool, const SOCKADDR& sender);
		void DumpTagInto(Pool& pool, TagID tag, const SOCKADDR& sender);
	};

	template<Math::Number Number>
	bool Packet::TryInject(Number number, bool leaveOpen) noexcept
	{
		if (holes.empty())
			return false;
		auto& hole = holes.front();
		switch (hole.size)
		{
		case 1:
			if constexpr (std::is_same_v<Number, sf::Uint8> && std::is_same_v<sf::Uint8, Number>)
				payload[hole.position] = static_cast<char>(number);
			else
				return false;
			break;
		case 2:
			if constexpr (std::is_same_v<Number, sf::Uint16> && std::is_same_v<sf::Uint16, Number>)
			{
				auto networkNumber{ htons(number) };
				std::memcpy(&payload[hole.position], std::bit_cast<char*>(&networkNumber), hole.size);
			}
			else
				return false;
			break;
		case 4:
			if constexpr (std::is_same_v<Number, float> && std::is_same_v<float, Number>)
			{
				auto networkNumber{ htonf(number) };
				std::memcpy(&payload[hole.position], std::bit_cast<char*>(&networkNumber), hole.size);
			}
			else
				return false;
			break;
		default:
			return false;
		}
		if (!leaveOpen)
			holes.pop();
		return true;
	}

	void Network::Packet::DumpHandshakeMessageInto(Message auto message, 
		Pool& pool, const SOCKADDR& sender)
	{
		HandshakeProperties properties{ static_cast<HandshakeProperties>(payload[1]) };
		std::optional<sf::Uint8> version{ std::nullopt };
		if ((properties & HandshakeProperties::Versions) != HandshakeProperties::None)
			version = payload[2 + message.Size()];
		pool.Emplace(std::move(message), properties, sender, version);
	}

	void Network::Packet::DumpMessageInto(Message auto message, 
		Pool& pool, const SOCKADDR& sender)
	{
		MessageProperties properties{ static_cast<MessageProperties>(payload[1]) };
		std::optional<float> timestamp{ std::nullopt };
		std::optional<sf::Uint8> version{ std::nullopt };
		GetOptionalData(timestamp, version, 2 + message.Size());
		pool.Emplace(std::move(message), properties, sender, timestamp, version);
	}
}
