// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/PlatformMacros.h"
#include "Net/Sockets/AddressFamily.h"
#include "Net/Sockets/SocketType.h"
#include "Net/Sockets/SocketShutdown.h"
#include "Net/IPEndPoint.h"
#include "Threading/Tasks/Task.h"
#include <memory>
#include <span>
#include <stop_token>

namespace Ayla
{
	class CORE_API Socket
	{
		Socket(const Socket&) = delete;
		Socket(Socket&&) = delete;
		Socket& operator =(const Socket&) = delete;
		Socket& operator =(Socket&&) = delete;

	private:
		struct PlatformSocket;

	private:
		std::unique_ptr<PlatformSocket> m_Socket;

	private:
		static void Initialize();
		Socket(std::unique_ptr<PlatformSocket> socket);

	public:
		Socket(AddressFamily af, SocketType st);
		~Socket() noexcept;

		// Basic socket operations
		void Close();
		void Bind(const IPEndPoint& localEP);
		void Listen();
		void Listen(int32 backlog);
		void Connect(const IPEndPoint& remoteEP);
		std::shared_ptr<Socket> Accept();

		// Synchronous I/O
		size_t Send(std::span<const uint8> buffer);
		size_t Receive(std::span<uint8> buffer);
		size_t SendTo(std::span<const uint8> buffer, const IPEndPoint& remoteEP);
		size_t ReceiveFrom(std::span<uint8> buffer, IPEndPoint& remoteEP);

		// Asynchronous operations using Task
		Task<std::shared_ptr<Socket>> AcceptAsync(std::stop_token cancellationToken = {});
		Task<> ConnectAsync(const IPEndPoint& remoteEP, std::stop_token cancellationToken = {});
		Task<size_t> SendAsync(std::span<const uint8> buffer, std::stop_token cancellationToken = {});
		Task<size_t> ReceiveAsync(std::span<uint8> buffer, std::stop_token cancellationToken = {});
		Task<size_t> SendToAsync(std::span<const uint8> buffer, const IPEndPoint& remoteEP, std::stop_token cancellationToken = {});
		Task<size_t> ReceiveFromAsync(std::span<uint8> buffer, IPEndPoint& remoteEP, std::stop_token cancellationToken = {});

		// Properties
		bool IsConnected() const;
		bool IsBound() const;
		bool IsListening() const;
		AddressFamily GetAddressFamily() const;
		SocketType GetSocketType() const;
		IPEndPoint GetLocalEndPoint() const;
		IPEndPoint GetRemoteEndPoint() const;

		// Socket options
		void SetSocketOption(int32 level, int32 optionName, bool optionValue);
		void SetSocketOption(int32 level, int32 optionName, int32 optionValue);
		void SetSocketOption(int32 level, int32 optionName, std::span<const uint8> optionValue);
		bool GetSocketOptionBool(int32 level, int32 optionName) const;
		int32 GetSocketOptionInt32(int32 level, int32 optionName) const;
		std::vector<uint8> GetSocketOptionBytes(int32 level, int32 optionName) const;

		void Shutdown(SocketShutdown how);
		int32 GetAvailable() const;
		bool Poll(int32 microSeconds, int32 mode) const;

	private:
		// Internal helper methods
		void EnsureSocket() const;
		static std::shared_ptr<Socket> CreateFromPlatformSocket(std::unique_ptr<PlatformSocket> platformSocket);
	};
}