// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Net/Sockets/Socket.h"
#include "ArgumentException.h"
#include "SystemException.h"
#include "Net/IPAddress.h"

#if PLATFORM_LINUX || PLATFORM_OSX
#include "Socket.Unix.inl"
#elif PLATFORM_WINDOWS
#include "Socket.Windows.inl"
#endif

namespace Ayla
{
	void Socket::Initialize()
	{
		// Ensure that the platform-specific socket subsystem is initialized before any sockets are created.
		static int dummy = (PlatformSocket::Initialize(), 0);
	}

	Socket::Socket(std::unique_ptr<PlatformSocket> socket)
		: m_Socket{ std::move(socket) }
	{
	}

	Socket::Socket(AddressFamily af, SocketType st)
		: Socket((Initialize(), std::make_unique<PlatformSocket>(af, st)))
	{
	}

	Socket::~Socket() noexcept
	{
	}

	void Socket::Close()
	{
		m_Socket.reset();
	}

	void Socket::Bind(const IPEndPoint& localEP)
	{
		EnsureSocket();
		m_Socket->Bind(localEP);
	}

	void Socket::Listen(int32 backlog)
	{
		EnsureSocket();
		m_Socket->Listen(backlog);
	}

	void Socket::Connect(const IPEndPoint& remoteEP)
	{
		EnsureSocket();
		m_Socket->Connect(remoteEP);
	}

	std::shared_ptr<Socket> Socket::Accept()
	{
		EnsureSocket();
		auto clientSocket = m_Socket->Accept();
		return CreateFromPlatformSocket(std::move(clientSocket));
	}

	size_t Socket::Send(std::span<const uint8> buffer)
	{
		EnsureSocket();
		return m_Socket->Send(buffer);
	}

	size_t Socket::Receive(std::span<uint8> buffer)
	{
		EnsureSocket();
		return m_Socket->Receive(buffer);
	}

	size_t Socket::SendTo(std::span<const uint8> buffer, const IPEndPoint& remoteEP)
	{
		EnsureSocket();
		return m_Socket->SendTo(buffer, remoteEP);
	}

	size_t Socket::ReceiveFrom(std::span<uint8> buffer, IPEndPoint& remoteEP)
	{
		EnsureSocket();
		return m_Socket->ReceiveFrom(buffer, remoteEP);
	}

	Task<std::shared_ptr<Socket>> Socket::AcceptAsync(std::stop_token cancellationToken)
	{
		EnsureSocket();
		return m_Socket->AcceptAsync(cancellationToken);
	}

	Task<> Socket::ConnectAsync(const IPEndPoint& remoteEP, std::stop_token cancellationToken)
	{
		EnsureSocket();
		return m_Socket->ConnectAsync(remoteEP, cancellationToken);
	}

	Task<size_t> Socket::SendAsync(std::span<const uint8> buffer, std::stop_token cancellationToken)
	{
		EnsureSocket();
		return m_Socket->SendAsync(buffer, cancellationToken);
	}

	Task<size_t> Socket::ReceiveAsync(std::span<uint8> buffer, std::stop_token cancellationToken)
	{
		EnsureSocket();
		return m_Socket->ReceiveAsync(buffer, cancellationToken);
	}

	Task<size_t> Socket::SendToAsync(std::span<const uint8> buffer, const IPEndPoint& remoteEP, std::stop_token cancellationToken)
	{
		EnsureSocket();
		return m_Socket->SendToAsync(buffer, remoteEP, cancellationToken);
	}

	Task<size_t> Socket::ReceiveFromAsync(std::span<uint8> buffer, IPEndPoint& remoteEP, std::stop_token cancellationToken)
	{
		EnsureSocket();
		return m_Socket->ReceiveFromAsync(buffer, remoteEP, cancellationToken);
	}

	bool Socket::IsConnected() const
	{
		return m_Socket && m_Socket->IsConnected();
	}

	bool Socket::IsBound() const
	{
		return m_Socket && m_Socket->IsBound();
	}

	bool Socket::IsListening() const
	{
		return m_Socket && m_Socket->IsListening();
	}

	AddressFamily Socket::GetAddressFamily() const
	{
		EnsureSocket();
		return m_Socket->GetAddressFamily();
	}

	SocketType Socket::GetSocketType() const
	{
		EnsureSocket();
		return m_Socket->GetSocketType();
	}

	IPEndPoint Socket::GetLocalEndPoint() const
	{
		EnsureSocket();
		return m_Socket->GetLocalEndPoint();
	}

	IPEndPoint Socket::GetRemoteEndPoint() const
	{
		EnsureSocket();
		return m_Socket->GetRemoteEndPoint();
	}

	void Socket::SetSocketOption(int32 level, int32 optionName, bool optionValue)
	{
		EnsureSocket();
		m_Socket->SetSocketOption(level, optionName, optionValue);
	}

	void Socket::SetSocketOption(int32 level, int32 optionName, int32 optionValue)
	{
		EnsureSocket();
		m_Socket->SetSocketOption(level, optionName, optionValue);
	}

	void Socket::SetSocketOption(int32 level, int32 optionName, std::span<const uint8> optionValue)
	{
		EnsureSocket();
		m_Socket->SetSocketOption(level, optionName, optionValue);
	}

	bool Socket::GetSocketOptionBool(int32 level, int32 optionName) const
	{
		EnsureSocket();
		return m_Socket->GetSocketOptionBool(level, optionName);
	}

	int32 Socket::GetSocketOptionInt32(int32 level, int32 optionName) const
	{
		EnsureSocket();
		return m_Socket->GetSocketOptionInt32(level, optionName);
	}

	std::vector<uint8> Socket::GetSocketOptionBytes(int32 level, int32 optionName) const
	{
		EnsureSocket();
		return m_Socket->GetSocketOptionBytes(level, optionName);
	}

	void Socket::Shutdown(SocketShutdown how)
	{
		EnsureSocket();
		m_Socket->Shutdown(how);
	}

	int32 Socket::GetAvailable() const
	{
		EnsureSocket();
		return m_Socket->GetAvailable();
	}

	bool Socket::Poll(int32 microSeconds, int32 mode) const
	{
		EnsureSocket();
		return m_Socket->Poll(microSeconds, mode);
	}

	void Socket::EnsureSocket() const
	{
		if (!m_Socket)
		{
#if PLATFORM_WINDOWS
			throw SystemException(WSAENOTCONN, TEXT("Socket is not initialized or has been closed"));
#else
			throw SystemException(ENOTCONN, TEXT("Socket is not initialized or has been closed"));
#endif
		}
	}

	std::shared_ptr<Socket> Socket::CreateFromPlatformSocket(std::unique_ptr<PlatformSocket> platformSocket)
	{
		return std::shared_ptr<Socket>(new Socket(std::move(platformSocket)));
	}
}