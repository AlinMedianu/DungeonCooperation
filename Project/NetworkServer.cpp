#include "NetworkServer.h"

using namespace std::chrono_literals;

namespace Network
{
    Server::Server(const std::string& boundAddressAndPortFileName)
        : socket(10000ms), transmitter(socket), receiver(socket),
        clients{ std::nullopt, std::nullopt }, toSend{},
        floorBuildingSeed{ std::nullopt }, clientPositions{ std::nullopt, 
        std::nullopt }, disconnected{}, sentDisconnectionMessages{}
    {
        std::string address{};
        sf::Uint16 port{};
        socket.Bind(address, port);
        std::ofstream boundAddressAndPortFile(boundAddressAndPortFileName);
        boundAddressAndPortFile << address << '\n' << port;
        boundAddressAndPortFile.close();
        receiver.Start();
    }

    bool Server::IsDisconnected() const
    {
        return disconnected;
    }

    bool Server::IsClosed() const
    {
        return transmitter.IsIdle();
    }

    void Server::Update()
    {
        disconnected = receiver.WantsToReset();
        transmitter.Update();
        ExtendedMessage<Synchronize, HandshakeProperties> syncrhonize{};
        if (receiver.HasReceived(syncrhonize))
        {
            if (!IsAcknowledged(syncrhonize))
                transmitter.Answer(syncrhonize);
            else
            {
                for (auto& client : clients)
                    if (!client)
                    {
                        client.emplace(syncrhonize.sender);
                        break;
                    }
            }
        }
        ExtendedMessage<Tag, MessageProperties> tag{};
        if (receiver.HasReceived(tag) && IsDisconnectionMessage(tag))
            transmitter.StopRetransmitting(tag);
    }

    void Server::Close()
    {
        if (sentDisconnectionMessages)
            return;
        for (auto& client : clients)
            if (client)
            {
                transmitter.Disconnect(client.value());
                sentDisconnectionMessages = true;
            }
    }
}