#pragma once

#include <string_view>
#include <chrono>
#include <bit>
#include "NetworkPacket.h"

namespace Network
{
	class Socket
	{
		std::string errorMessage;
		WSADATA implementation;
		SOCKET socketID;
		std::chrono::milliseconds receiveTimeout;
	public:
		explicit Socket(std::chrono::milliseconds receiveTimeout);
		void Bind(std::string& boundAddress, sf::Uint16& boundPort);
		[[nodiscard]] bool HasErrors() const noexcept;
		[[nodiscard]] std::string_view GetErrorMessage() const noexcept;
		bool Send(const Packet& packet, const SOCKADDR& to);
		bool Receive(Packet& packet, SOCKADDR& from);
		~Socket();
	};
}
