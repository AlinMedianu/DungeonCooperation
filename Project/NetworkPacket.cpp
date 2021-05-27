#include "NetworkPacket.h"

namespace Network
{
    Packet::Packet() noexcept
        : payload(maxPacketSize, '\0'), holes{}
    {

    }

    void Packet::Process(HandshakeProperties properties, HandshakeMessageID message)
    {
        payload[0] = static_cast<sf::Uint8>(message);
        MessageProperties messageProperties{ MessageProperties::Reliable };
        if ((properties & HandshakeProperties::Acknowledged) != HandshakeProperties::None)
            messageProperties &= MessageProperties::Acknowledged;
        if ((properties & HandshakeProperties::Rejected) != HandshakeProperties::None)
            messageProperties &= MessageProperties::Rejected;
        if ((properties & HandshakeProperties::Versions) != HandshakeProperties::None)
            messageProperties &= MessageProperties::Versions;
        payload[1] = static_cast<sf::Uint8>(properties);
        holes.emplace(2, 2);
        switch (message)
        {
        case HandshakeMessageID::Synchronize:
            payload.resize(4);
            break;
        case HandshakeMessageID::SynchronizeAcknowledgement:
            holes.emplace(4, 2);
            payload.resize(6);
            break;
        }
    }

    void Packet::Process(KeepAlive message) noexcept
    {
        payload.clear();
    }

    void Packet::Process(MessageProperties properties, TagID message)
    {
        payload[0] = static_cast<sf::Uint8>(MessageID::FirstTag) + static_cast<sf::Uint8>(message);
        payload[1] = From(properties);
        ReserveSpaceForOptionalData(properties, 2);
    }

    void Packet::Process(MessageProperties properties, Seed message)
    {
        payload[0] = static_cast<sf::Uint8>(MessageID::Seed);
        payload[1] = From(properties);
        auto seed{ htonl(message.seed) };
        std::memcpy(&payload[2], std::bit_cast<char*>(&seed), 4);
        ReserveSpaceForOptionalData(properties, 6);
    }

    void Packet::Process(MessageProperties properties, Position message)
    {
        payload[0] = static_cast<sf::Uint8>(MessageID::Position);
        payload[1] = From(properties);
        auto positionX{ htonf(message.position.x) };
        auto positionY{ htonf(message.position.y) };
        std::memcpy(&payload[2], std::bit_cast<char*>(&positionX), 4);
        std::memcpy(&payload[6], std::bit_cast<char*>(&positionY), 4);
        ReserveSpaceForOptionalData(properties, 10);
    }

    void Packet::Clear()
    {
        assert(holes.size() == 0);
        payload.clear();
        payload.resize(maxPacketSize);
    }

    MessageProperties Packet::GetProperties() const
    {
        return static_cast<MessageProperties>(payload[1]);
    }

    std::string_view Packet::GetPayload() const noexcept
    {
        return payload;
    }

    std::string& Packet::GetPayload() noexcept
    {
        return payload;
    }

    void Packet::DumpInto(Pool& pool, const SOCKADDR& sender)
    {
        if (payload.size() == 0)
        {
            pool.Push(sender);
            return;
        }
        switch (static_cast<MessageID>(payload[0]))
        {
        case MessageID::Synchronize:
        {
            auto synchronize{ *std::bit_cast<sf::Uint16*>(&payload[2]) };
            Synchronize message
            {
                .synchronize = ntohs(synchronize)
            };
            DumpHandshakeMessageInto(message, pool, sender);
        }
            break;
        case MessageID::SynchronizeAcknowledgement:
        {
            auto synchronize{ *std::bit_cast<sf::Uint16*>(&payload[2]) };
            auto acknowledgement{ *std::bit_cast<sf::Uint16*>(&payload[4]) };
            SynchronizeAcknowledgement message
            {
                .synchronize = ntohs(synchronize),
                .acknowledgement = ntohs(acknowledgement)
            };
            DumpHandshakeMessageInto(message, pool, sender);
        }
            break;
        case static_cast<MessageID>(static_cast<sf::Uint8>(MessageID::FirstTag) + 
            static_cast<sf::Uint8>(TagID::Disconnect)):
            DumpTagInto(pool, TagID::Disconnect, sender);
            break;
        case MessageID::Seed:
        {
            auto seed{ *std::bit_cast<int*>(&payload[2]) };
            Seed message
            {
                .seed = static_cast<int>(ntohl(seed))
            };
            DumpMessageInto(message, pool, sender);
        }
            break;
        case MessageID::Position:
        {
            auto positionX{ *std::bit_cast<sf::Uint32*>(&payload[2]) };
            auto positionY{ *std::bit_cast<sf::Uint32*>(&payload[6]) };
            Position message
            {
                .position{ ntohf(positionX), ntohf(positionY) }
            };
            DumpMessageInto(message, pool, sender);
        }
            break;
        }
    } 

    constexpr sf::Uint8 Packet::From(MessageProperties properties) const noexcept
    {
        return static_cast<sf::Uint8>(properties);
    }

    void Packet::ReserveSpaceForOptionalData(MessageProperties properties, sf::Uint8 from)
    {
        if ((properties & MessageProperties::ClientTimestamp) != MessageProperties::None)
        {
            holes.emplace(from, 4);
            from += 4;
        }
        if ((properties & MessageProperties::Versions) != MessageProperties::None)
            holes.emplace(from++, 1);
        payload.resize(from);
    }

    void Packet::GetOptionalData(std::optional<float>& timestamp, 
        std::optional<sf::Uint8>& version, sf::Uint8 optionalDataStart) const
    {
        MessageProperties properties{ GetProperties() };
        sf::Uint8 currentPosition{ optionalDataStart };
        if ((properties & MessageProperties::ClientTimestamp) != MessageProperties::None)
        {
            auto networkTimestamp{ *std::bit_cast<sf::Uint32*>(&payload[currentPosition]) };
            timestamp = ntohf(networkTimestamp);
            currentPosition += 4;
        }
        if ((properties & MessageProperties::Versions) != MessageProperties::None)
            version = payload[currentPosition++];
    }

    void Packet::DumpTagInto(Pool& pool, TagID tag, const SOCKADDR& sender)
    {
        DumpMessageInto(Tag { .id = tag }, pool, sender);
    }
}