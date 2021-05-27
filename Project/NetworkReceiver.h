#pragma once

#include <thread>
#include <optional>
#include "NetworkPool.h"
#include "NetworkSocket.h"

namespace Network
{
	class Receiver
	{
		Socket& receiver;
		Trap trap;
		std::thread receiverThread;
		bool running;
		void Receive();
	public:
		Receiver(Socket& receiver) noexcept;
		[[nodiscard]] bool WantsToReset() const noexcept;
		[[nodiscard]] bool HasReceived(ExtendedMessage<Synchronize, HandshakeProperties>& message) const noexcept;
		[[nodiscard]] bool HasReceived(ExtendedMessage<SynchronizeAcknowledgement, HandshakeProperties>& message) const noexcept;
		[[nodiscard]] bool HasReceived(ExtendedMessage<Tag, MessageProperties>& message) const noexcept;
		[[nodiscard]] bool HasReceived(ExtendedMessage<Seed, MessageProperties>& message) const noexcept;
		[[nodiscard]] bool HasReceived(ExtendedMessage<Position, MessageProperties>& message) const noexcept;
		[[nodiscard]] bool HasReceived(SOCKADDR& heartBeat) const noexcept;
		void Start();
		void Disconnect() noexcept;
		~Receiver();
	};

	[[nodiscard]] inline bool IsDisconnectionMessage(
		const ExtendedMessage<Tag, MessageProperties>& message) noexcept
	{
		return message.original.id == TagID::Disconnect;
	}

	[[nodiscard]] inline bool IsAcknowledged(
		const ExtendedMessage<Synchronize, HandshakeProperties>& message) noexcept
	{
		return (message.properties & HandshakeProperties::Acknowledged) != HandshakeProperties::None;
	}
}
