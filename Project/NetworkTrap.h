#pragma once

#include "NetworkMessages.h"
#include <winsock2.h>

namespace Network
{
	template<Message Wrapped, BitMask Properties>
	struct ExtendedMessage
	{
		Wrapped original;
		Properties properties;
		SOCKADDR sender;
		std::optional<float> timestamp;
		std::optional<sf::Uint8> version;
	};

	enum class ReadOrWriteMode
	{
		Read, Write, COUNT
	};

	template<typename T>
	class ReadOrWrite
	{
		T data;
		mutable ReadOrWriteMode mode;
	public:
		ReadOrWrite() noexcept;
		bool TryRead(T& data) const noexcept;
		bool TryWrite(const T& data) noexcept;
	};

	template<typename T>
	inline ReadOrWrite<T>::ReadOrWrite() noexcept
		: data{}, mode{ ReadOrWriteMode::Write }
	{

	}

	template<typename T>
	inline bool ReadOrWrite<T>::TryRead(T& data) const noexcept
	{
		if (mode == ReadOrWriteMode::Read)
		{
			data = this->data;
			mode = ReadOrWriteMode::Write;
			return true;
		}
		return false;
	}

	template<typename T>
	inline bool ReadOrWrite<T>::TryWrite(const T& data) noexcept
	{
		if (mode == ReadOrWriteMode::Write)
		{
			this->data = data;
			mode = ReadOrWriteMode::Read;
			return true;
		}
		return false;
	}

	struct Trap
	{
		ReadOrWrite<ExtendedMessage<Synchronize, HandshakeProperties>> syncronization;
		ReadOrWrite<ExtendedMessage<SynchronizeAcknowledgement, HandshakeProperties>> syncronizeAcknowledgement;
		ReadOrWrite<ExtendedMessage<Tag, MessageProperties>> tag;
		ReadOrWrite<ExtendedMessage<Seed, MessageProperties>> seed;
		ReadOrWrite<ExtendedMessage<Position, MessageProperties>> position;
		ReadOrWrite<SOCKADDR> heartBeat;
	};
}

