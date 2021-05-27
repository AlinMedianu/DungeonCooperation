#pragma once

#include <fstream>
#include "NetworkMaxConnections.h"
#include "NetworkTransmitter.h"
#include "NetworkReceiver.h"

namespace Network
{
	
	class Server
	{
		Socket socket;
		Transmitter transmitter;
		Receiver receiver;
		std::array<std::optional<SOCKADDR>, maxConnections> clients;
		Packet toSend;
		std::optional<Seed> floorBuildingSeed;
		std::array<std::optional<Position>, maxConnections> clientPositions;
		bool disconnected, sentDisconnectionMessages;
		const SOCKADDR& GetOther() const noexcept;
	public:
		Server(const std::string& boundAddressAndPortFileName);
		[[nodiscard]] bool IsDisconnected() const;
		[[nodiscard]] bool IsClosed() const;
		void Update();
		void AdjustClientPositions();
		void Close();
	};
}
