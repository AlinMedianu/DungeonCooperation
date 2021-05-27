#include "NetworkTransmitter.h"

namespace Network
{
    void Transmitter::InjectRandomNumber(Packet& in)
    {
        assert(in.TryInject(static_cast<sf::Uint16>(numberGenerator(
            std::numeric_limits<sf::Uint16>().min(),
            std::numeric_limits<sf::Uint16>().max()))));
    }

    void Transmitter::SendReliably(Packet& packet, const SOCKADDR& to)
    {
        auto resender{ std::find_if(resenders.begin(), resenders.end(),
            [](const Retransmitter& resender)
            {
                return !resender.IsRunning();
            }) };
        if (resender != resenders.end())
            resender->Start(packet, to);
        else
            resenders.emplace_back(sender, timeoutCalculator).Start(packet, to);
        packet.Clear();

    }

    void Transmitter::StopRetransmitting(const MessageID& message)
    {
        for (auto& resender : resenders)
            if (resender.GetPacket().GetPayload()[0] == static_cast<char>(message))
                timeoutCalculator.UpdateFrom(milliseconds{ timer.getElapsedTime().asMilliseconds() } -
                    resender.StopAndGetTimeSinceStart());
    }

    Transmitter::Transmitter(Socket& sender) noexcept
        : sender(sender), timeoutCalculator(), resenders{}, numberGenerator{}, timer(),
        heartBeatTimer{ std::nullopt }, idle{ false }, disconnectionMessagesSent{}
    {
        resenders.reserve(4);
    }

    void Transmitter::StartSynchronizing(const SOCKADDR& with)
    {
        idle = false;
        Packet handshakeMessage;
        handshakeMessage.Process(HandshakeProperties::None, HandshakeMessageID::Synchronize);
        InjectRandomNumber(handshakeMessage);
        SendReliably(handshakeMessage, with);
    }

    void Transmitter::Reject(ExtendedMessage<Synchronize, HandshakeProperties>& message)
    {
        Packet sinchronizeMessage;
        sinchronizeMessage.Process(HandshakeProperties::Rejected, HandshakeMessageID::Synchronize);
        assert(sinchronizeMessage.TryInject(message.original.synchronize));
        assert(sender.Send(sinchronizeMessage, message.sender));
    }

    void Transmitter::Answer(const ExtendedMessage<Synchronize, HandshakeProperties>& message)
    {
        if ((message.properties & HandshakeProperties::Acknowledged) == HandshakeProperties::None)
        {
            idle = false;
            Packet handshakeMessage;
            handshakeMessage.Process(HandshakeProperties::Versions,
                HandshakeMessageID::SynchronizeAcknowledgement);
            InjectRandomNumber(handshakeMessage);
            assert(handshakeMessage.TryInject(static_cast<sf::Uint16>(message.original.synchronize + 1)));
            SendReliably(handshakeMessage, message.sender);
        }
        else
        {
            StopRetransmitting(MessageID::SynchronizeAcknowledgement);
            if (!heartBeatTimer)
                heartBeatTimer.emplace();
        }
    }

    bool Transmitter::Answer(const ExtendedMessage<SynchronizeAcknowledgement, HandshakeProperties>& message)
    {
        Packet handshakeMessage;
        handshakeMessage.Process(HandshakeProperties::Acknowledged, HandshakeMessageID::Synchronize);
        assert(handshakeMessage.TryInject(static_cast<sf::Uint16>(message.original.synchronize + 1)));
        StopRetransmitting(MessageID::Synchronize);
        return sender.Send(handshakeMessage, message.sender);
    }

    bool Transmitter::Send(Packet& packet, const SOCKADDR& to)
    {
        auto properties{ packet.GetProperties() };
        if ((properties & MessageProperties::ClientTimestamp) != MessageProperties::None)
            assert(packet.TryInject(static_cast<float>(timer.getElapsedTime().asMilliseconds())));
        if ((properties & MessageProperties::Reliable) == MessageProperties::None)
        {
            bool errorless{ sender.Send(packet, to) };
            packet.Clear();
            return errorless;
        }
        SendReliably(packet, to);
        return true;
    }

    bool Transmitter::IsIdle() const noexcept
    {
        return idle;
    }

    bool Transmitter::ShouldSendKeepAliveMessage() const
    {
        return heartBeatTimer && heartBeatTimer->getElapsedTime().asSeconds() >= 1.0f;
    }

    void Transmitter::Update()
    {
        for (auto& resender : resenders)
            resender.Update();
    }

    void Transmitter::SendKeepAliveMessage(const SOCKADDR& to)
    {
        Packet keepAliveMessage;
        keepAliveMessage.Process(KeepAlive{});
        sender.Send(keepAliveMessage, to);
    }

    void Transmitter::RestartHeartBeatTimer()
    {
        heartBeatTimer->restart();
    } 

    void Transmitter::Disconnect(const SOCKADDR& from)
    {
        Packet dissconnectionMessage;
        dissconnectionMessage.Process(MessageProperties::Reliable, TagID::Disconnect);
        SendReliably(dissconnectionMessage, from);
        heartBeatTimer.reset();
        ++disconnectionMessagesSent;
    }

    void Transmitter::Reset()
    {
        heartBeatTimer.reset();
        disconnectionMessagesSent = 0;
    }
}