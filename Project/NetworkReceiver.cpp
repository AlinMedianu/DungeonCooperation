#include "NetworkReceiver.h"

namespace Network
{
    void Receiver::Receive()
    {
        Pool pool{};
        Packet received{};
        SOCKADDR sender{};
        while (running)
            if (receiver.Receive(received, sender))
            {
                received.DumpInto(pool, sender);
                pool.TrapMessages(trap);
            }
            else switch (WSAGetLastError())
            {
            case WSAETIMEDOUT:
                running = false;
                break;
            }
    }

    Receiver::Receiver(Socket& receiver) noexcept
        : receiver(receiver), trap{}, receiverThread(), running{}
    {

    }

    bool Receiver::WantsToReset() const noexcept
    {
        return !running;
    }

    bool  Receiver::HasReceived(ExtendedMessage<Synchronize, HandshakeProperties>& message) const noexcept
    {
        return trap.syncronization.TryRead(message);
    }

    bool  Receiver::HasReceived(ExtendedMessage<SynchronizeAcknowledgement, HandshakeProperties>& message) const noexcept
    {
        return trap.syncronizeAcknowledgement.TryRead(message);
    }

    bool  Receiver::HasReceived(ExtendedMessage<Tag, MessageProperties>& message) const noexcept
    {
        return trap.tag.TryRead(message);
    }

    bool  Receiver::HasReceived(ExtendedMessage<Seed, MessageProperties>& message) const noexcept
    {
        return trap.seed.TryRead(message);
    }

    bool  Receiver::HasReceived(ExtendedMessage<Position, MessageProperties>& message) const noexcept
    {
        return trap.position.TryRead(message);
    }

    bool Receiver::HasReceived(SOCKADDR& heartBeat) const noexcept
    {
        return trap.heartBeat.TryRead(heartBeat);
    }

    void Receiver::Start()
    {
        running = true;
        receiverThread = std::thread(&Receiver::Receive, this);
    }

    void Receiver::Disconnect() noexcept
    {
        running = false;
        if (receiverThread.joinable())
            receiverThread.join();
    }

    Receiver::~Receiver()
    {
        if (receiverThread.joinable())
            receiverThread.join();
    }
}