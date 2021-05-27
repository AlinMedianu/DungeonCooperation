#include "NetworkClient.h"

using namespace std::chrono_literals;

namespace Network
{
    Client::Client(const std::string& serverAddressAndPortFileName)
        : socket(2000ms), transmitter(socket), receiver(socket),
        server{ std::nullopt }, toSend(), floorBuildingSeed{ std::nullopt },
        startingClientPositions{ std::nullopt, std::nullopt }
    {
        std::ifstream serverAddressAndPortFile(
            serverAddressAndPortFileName, std::ios_base::in);
        SOCKADDR_IN serverAddressAndPort
        {
            .sin_family = AF_INET,
        };
        std::string serverAddress{};
        sf::Uint16 serverPort{};
        std::getline(serverAddressAndPortFile, serverAddress);
        serverAddressAndPort.sin_addr.s_addr = inet_addr(serverAddress.c_str());
        serverAddressAndPortFile >> serverPort;
        serverAddressAndPortFile.close();
        serverAddressAndPort.sin_port = htons(serverPort);
        server.emplace(*std::bit_cast<SOCKADDR*>(&serverAddressAndPort));
        transmitter.StartSynchronizing(server.value());
        receiver.Start();
    }

    void Client::Update()
    {
        transmitter.Update();
        ExtendedMessage<SynchronizeAcknowledgement, HandshakeProperties> syncrhonizeAcknowledgement{};
        if (receiver.HasReceived(syncrhonizeAcknowledgement))
            transmitter.Answer(syncrhonizeAcknowledgement);
        ExtendedMessage<Tag, MessageProperties> tag{};
        if (receiver.HasReceived(tag) && IsDisconnectionMessage(tag))
            transmitter.Acknowledge(tag);
    }
}