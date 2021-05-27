#pragma once

#include <vector>
#include <algorithm>
#include <limits>
#include "NetworkRetransmitter.h"
#include "RandomNumberGenerator.h"

namespace Network
{
	class Transmitter
	{
		Socket& sender;
		RetransmissionTimeoutCalculator timeoutCalculator;
		std::vector<Retransmitter> resenders;
		RandomNumberGenerator numberGenerator;
		sf::Clock timer;
		std::optional<sf::Clock> heartBeatTimer;
		bool idle;
		sf::Uint8 disconnectionMessagesSent;
		void InjectRandomNumber(Packet& in);
		void SendReliably(Packet& packet, const SOCKADDR& to);
		void StopRetransmitting(const MessageID& message);
	public:
		Transmitter(Socket& sender) noexcept;
		void StartSynchronizing(const SOCKADDR& with);
		void Reject(ExtendedMessage<Synchronize, HandshakeProperties>& message);
		void Answer(const ExtendedMessage<Synchronize, HandshakeProperties>& message);
		bool Answer(const ExtendedMessage<SynchronizeAcknowledgement, HandshakeProperties>& message);
		bool Send(Packet& packet, const SOCKADDR& to);
		template<Message Wrapped>
		inline void Acknowledge(ExtendedMessage<Wrapped, MessageProperties>& message);
		template<>
		inline void Acknowledge(ExtendedMessage<Tag, MessageProperties>& message);
		[[nodiscard]] bool IsIdle() const noexcept;
		[[nodiscard]] bool ShouldSendKeepAliveMessage() const;
		void Update();
		void SendKeepAliveMessage(const SOCKADDR& to);
		void RestartHeartBeatTimer();
		template<Message Wrapped>
		void StopRetransmitting(const ExtendedMessage<Wrapped, MessageProperties>& acknowledgement);
		template<>
		void StopRetransmitting(const ExtendedMessage<Tag, MessageProperties>& acknowledgement);
		void Disconnect(const SOCKADDR& from);
		void Reset();
	};

	template<Message Wrapped>
	void Transmitter::Acknowledge(ExtendedMessage<Wrapped, MessageProperties>& message)
	{
		auto messageProperties{ message.properties };
		if ((messageProperties & (MessageProperties::Acknowledged | MessageProperties::Rejected))
			!= MessageProperties::None || (messageProperties & MessageProperties::Reliable) 
			== MessageProperties::None)
			return;
		Packet toSend;
		toSend.Process(message.properties, message.original);
		messageProperties |= MessageProperties::Acknowledged;
		toSend.GetPayload()[1] = static_cast<char>(messageProperties);
		sender.Send(toSend, message.sender);
	}

	template<>
	void Transmitter::Acknowledge(ExtendedMessage<Tag, MessageProperties>& message)
	{
		auto messageProperties{ message.properties };
		if ((messageProperties & (MessageProperties::Acknowledged | MessageProperties::Rejected))
			!= MessageProperties::None || (messageProperties & MessageProperties::Reliable)
			== MessageProperties::None)
			return;
		Packet toSend;
		toSend.Process(message.properties, message.original.id);
		messageProperties |= MessageProperties::Acknowledged;
		toSend.GetPayload()[1] = static_cast<char>(messageProperties);
		sender.Send(toSend, message.sender);
	}

	template<Message Wrapped>
	void Transmitter::StopRetransmitting(const ExtendedMessage<Wrapped, MessageProperties>& acknowledgement)
	{
		Packet basedOn;
		basedOn.Process(acknowledgement.properties, acknowledgement.original);
		for (auto& resender : resenders)
			if (resender.IsRunning() && *std::bit_cast<char*>(&resender.GetReceiver()) == *std::bit_cast<char*>(&acknowledgement.sender))
			{
				auto resenderPayload{ resender.GetPacket().GetPayload() };
				auto stopperPayload{ basedOn.GetPayload() };
				if (resenderPayload[0] == stopperPayload[0])
				{
					auto propertyDifferences{ acknowledgement.properties & ~resender.GetPacket().GetProperties() };
					if ((propertyDifferences & (MessageProperties::Acknowledged | MessageProperties::Rejected))
						== MessageProperties::None && std::equal(resenderPayload.begin() + 2,
							resenderPayload.end(), stopperPayload.begin()))
					{
						if (resender.GetPacket().GetPayload()[0] == static_cast<char>
							(MessageID::FirstTag) + static_cast<char>(TagID::Disconnect) &&
							--disconnectionMessagesSent == 0)
							idle = true;
						timeoutCalculator.UpdateFrom(milliseconds{ timer.getElapsedTime().asMilliseconds() } -
							resender.StopAndGetTimeSinceStart());
					}
				}
			}
	}

	template<>
	inline void Transmitter::StopRetransmitting(const ExtendedMessage<Tag, MessageProperties>& acknowledgement)
	{
		Packet basedOn;
		basedOn.Process(acknowledgement.properties, acknowledgement.original.id);
		for (auto& resender : resenders)
			if (resender.IsRunning() && *std::bit_cast<char*>(&resender.GetReceiver()) == *std::bit_cast<char*>(&acknowledgement.sender))
			{
				auto resenderPayload{ resender.GetPacket().GetPayload() };
				auto stopperPayload{ basedOn.GetPayload() };
				if (resenderPayload[0] == stopperPayload[0])
				{
					auto propertyDifferences{ acknowledgement.properties & ~resender.GetPacket().GetProperties() };
					if ((propertyDifferences & (MessageProperties::Acknowledged | MessageProperties::Rejected))
						== MessageProperties::None && std::equal(resenderPayload.begin() + 2,
							resenderPayload.end(), stopperPayload.begin()))
					{
						if (resender.GetPacket().GetPayload()[0] == static_cast<char>
							(MessageID::FirstTag)+static_cast<char>(TagID::Disconnect) &&
							--disconnectionMessagesSent == 0)
							idle = true;
						timeoutCalculator.UpdateFrom(milliseconds{ timer.getElapsedTime().asMilliseconds() } -
							resender.StopAndGetTimeSinceStart());
					}
				}
			}
	}
}
