#pragma once

#include <fstream>
#include "NetworkMaxConnections.h"
#include "NetworkTransmitter.h"
#include "NetworkReceiver.h"

namespace Network
{
	class Client
	{
		Socket socket;
		Transmitter transmitter;
		Receiver receiver;
		std::optional<SOCKADDR> server;
		Packet toSend;
		std::optional<Seed> floorBuildingSeed;
		std::array<std::optional<Position>, maxConnections> startingClientPositions;
	public:
		Client(const std::string& serverAddressAndPortFileName);
		void Update();
		[[nodiscard]] bool IsInGame() const;
		[[nodiscard]] bool IsOtherClientInGame() const;
		[[nodiscard]] bool IsClosed() const;
		const Seed& GetSeed();
		const Position& GetPosition() const;
		const Position& GetOtherClientPosition() const;
		[[nodiscard]] bool HasReceivedPosition(Position& position, bool& thisClient);
		void SendPosition(const Position& position);
		void Close();
	};
}