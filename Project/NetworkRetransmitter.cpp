#include "NetworkRetransmitter.h"

namespace Network
{
	void Retransmitter::Transmit()
	{
		if ((data.GetProperties() & MessageProperties::Versions) != MessageProperties::None)
			assert(this->data.TryInject(failedAttemptsCount, true));
		sender.Send(data, receiver);
		timeout = timeoutCalculator.GetTimeout(failedAttemptsCount++);
	}

	Retransmitter::Retransmitter(Socket& sender, const RetransmissionTimeoutCalculator& timeoutCalculator) noexcept
		: sender(sender), timeoutCalculator(timeoutCalculator), data(), receiver{}, 
		timeoutTimer{ std::nullopt }, runningTimer{ std::nullopt }, timeout{}, failedAttemptsCount{}
	{

	}

	bool Retransmitter::IsRunning() const noexcept
	{
		return runningTimer.has_value();
	}

	const Packet& Retransmitter::GetPacket() const noexcept
	{
		return data;
	}

	const SOCKADDR& Retransmitter::GetReceiver() const noexcept
	{
		return receiver;
	}

	void Retransmitter::Start(const Packet& data, const SOCKADDR& receiver)
	{
		this->data = data;		
		this->receiver = receiver;
		timeoutTimer.emplace();	
		runningTimer.emplace();
		Transmit();
	}

	void Retransmitter::Update()
	{
		if (!timeoutTimer || timeoutTimer->getElapsedTime().asMilliseconds() < timeout.count())
			return;
		Transmit();
		timeoutTimer->restart();
	}

	milliseconds Retransmitter::StopAndGetTimeSinceStart()
	{
		milliseconds runningFor{ runningTimer->getElapsedTime().asMilliseconds() };
		timeoutTimer.reset();
		runningTimer.reset();
		return runningFor;
	}
}