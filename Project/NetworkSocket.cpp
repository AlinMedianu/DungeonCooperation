#include "NetworkSocket.h"

namespace Network
{
	Socket::Socket(std::chrono::milliseconds receiveTimeout)
		: errorMessage{}, implementation {}, socketID{}, receiveTimeout(receiveTimeout)
	{
		if (WSAStartup(MAKEWORD(2, 2), &implementation) != 0)
		{
			errorMessage = "WSA Initialization failed! Error code: " +
				std::to_string(WSAGetLastError()) + "\n";
			return;
		}
		socketID = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (socketID == INVALID_SOCKET)
		{
			errorMessage = "Socket creation failed with error code: " + 
				std::to_string(WSAGetLastError()) + "!\n";
			return;
		}
		setsockopt(socketID, SOL_SOCKET, SO_RCVTIMEO, 
			std::bit_cast<char*>(&receiveTimeout), sizeof(DWORD));
	}

	void Socket::Bind(std::string& boundAddress, sf::Uint16& boundPort)
	{
		SOCKET connectedSocket = socket(AF_INET, SOCK_DGRAM, 0);
		if (connectedSocket == INVALID_SOCKET)
		{
			errorMessage = "Socket creation failed!";
			return;
		}
		SOCKADDR_IN address
		{
			.sin_family = AF_INET,
			.sin_port = 0
		};
		address.sin_addr.s_addr = INADDR_LOOPBACK;
		int length{ sizeof(SOCKADDR) };
		if (connect(connectedSocket, std::bit_cast<SOCKADDR*>(&address), length) == SOCKET_ERROR)
		{
			errorMessage = "Unable to connect socket! Error code: " +
				std::to_string(WSAGetLastError()) + "\r\n";
			closesocket(connectedSocket);
			return;
		}
		if (getsockname(connectedSocket, std::bit_cast<SOCKADDR*>(&address), &length) == SOCKET_ERROR)
		{
			errorMessage = "Couldn't retrieve the socket's IP address and port! Error code: " +
				std::to_string(WSAGetLastError()) + "\r\n";
			closesocket(connectedSocket);
			return;
		}
		closesocket(connectedSocket);
		boundAddress = std::string{ inet_ntoa(*std::bit_cast<in_addr*>(&address.sin_addr.s_addr)), sizeof(SOCKADDR) };
		boundPort = ntohs(address.sin_port);
		if (bind(socketID, std::bit_cast<SOCKADDR*>(&address), sizeof(SOCKADDR)) == SOCKET_ERROR)
			errorMessage = "Bind failed with error code: " + std::to_string(WSAGetLastError()) + "!\n";
	}

	bool Socket::HasErrors() const noexcept
	{
		return errorMessage.empty();
	}

	std::string_view Socket::GetErrorMessage() const noexcept
	{
		return { errorMessage.data(), errorMessage.size() };
	}

	bool Socket::Send(const Packet& packet, const SOCKADDR& to)
	{
		const std::string_view& payload(packet.GetPayload());
		return sendto(socketID, payload.data(), payload.size(),
			0, &to, sizeof(SOCKADDR)) != SOCKET_ERROR;
	}

	bool Socket::Receive(Packet& packet, SOCKADDR& from)
	{
		int addressSize{ sizeof(SOCKADDR) };
		std::string& payload(packet.GetPayload());
		int bytesReceived{ recvfrom(socketID, payload.data(), maxPacketSize, 0,
			&from, &addressSize) };
		if (bytesReceived == SOCKET_ERROR)
			return false;
		payload.resize(bytesReceived);
		return true;
	}

	Socket::~Socket()
	{
		shutdown(socketID, SD_BOTH);
		closesocket(socketID);
		WSACleanup();
	}
}