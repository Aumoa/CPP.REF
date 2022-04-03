// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SocketType.h"
#include "Net/AddressFamily.h"
#include "Net/ProtocolType.h"
#include "SocketFlags.h"
#include "Threading/Tasks/Task.h"

namespace libty::Sockets
{
	inline namespace Net
	{
		struct IPEndPoint;
	}

	class SOCKETS_API SSocket : virtual public SObject
	{
		GENERATED_BODY(SSocket);

	private:
		struct SocketImpl;
		std::shared_ptr<SocketImpl> Impl;

	private:
		SSocket();

	public:
		SSocket(EAddressFamily Family, ESocketType SocketType, EProtocolType ProtocolType);
		virtual ~SSocket() noexcept;

		bool IsValid() const;
		void Bind(const IPEndPoint& endpoint);
		void Close();
		void Listen(int32 backlog = 256);
		Task<SSocket*> Accept();

		size_t Recv(void* buf, size_t len, ESocketFlags flags = ESocketFlags::None, std::stop_token cancellationToken = {});

	private:
		bool IsReadyToRead(std::chrono::microseconds Timeout = 2048576ms);
		bool IsReadyToWrite(std::chrono::microseconds Timeout = 2048576ms);
	};
}