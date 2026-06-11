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
		sockaddr_storage addr;
		int addrLen = IPEndPointToSockAddr(localEP, addr);
		int result = bind(m_Socket->m_Socket, reinterpret_cast<sockaddr*>(&addr), addrLen);
		PlatformSocket::ThrowIfFailure(result);
		m_Socket->m_IsBound = true;
	}

	void Socket::Listen()
	{
		Listen(SOMAXCONN);
	}

	void Socket::Listen(int32 backlog)
	{
		EnsureSocket();
		int result = listen(m_Socket->m_Socket, backlog);
		PlatformSocket::ThrowIfFailure(result);
		m_Socket->m_IsListening = true;
	}

	void Socket::Connect(const IPEndPoint& remoteEP)
	{
		EnsureSocket();
		sockaddr_storage addr;
		int addrLen = IPEndPointToSockAddr(remoteEP, addr);
		int result = connect(m_Socket->m_Socket, reinterpret_cast<sockaddr*>(&addr), addrLen);
		PlatformSocket::ThrowIfFailure(result);
		m_Socket->m_IsConnected = true;
	}

	std::shared_ptr<Socket> Socket::Accept()
	{
		EnsureSocket();
		sockaddr_storage clientAddr;
		PlatformSocket::socket_len_type clientAddrLen = sizeof(clientAddr);
		auto clientSocket = accept(m_Socket->m_Socket, reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrLen);
		if (clientSocket == INVALID_SOCKET)
		{
			PlatformSocket::Throw();
		}
		return CreateFromPlatformSocket(std::make_unique<PlatformSocket>(clientSocket));
	}

	size_t Socket::Send(std::span<const uint8> buffer)
	{
		EnsureSocket();
		int result = send(m_Socket->m_Socket, reinterpret_cast<const char*>(buffer.data()),
			static_cast<int>(buffer.size()), 0);
		PlatformSocket::ThrowIfFailure(result);
		return static_cast<size_t>(result);
	}

	size_t Socket::Receive(std::span<uint8> buffer)
	{
		EnsureSocket();
		int result = recv(m_Socket->m_Socket, reinterpret_cast<char*>(buffer.data()),
			static_cast<int>(buffer.size()), 0);
		PlatformSocket::ThrowIfFailure(result);
		return static_cast<size_t>(result);
	}

	size_t Socket::SendTo(std::span<const uint8> buffer, const IPEndPoint& remoteEP)
	{
		EnsureSocket();
		sockaddr_storage addr;
		int addrLen = IPEndPointToSockAddr(remoteEP, addr);
		int result = sendto(m_Socket->m_Socket, reinterpret_cast<const char*>(buffer.data()),
			static_cast<int>(buffer.size()), 0, reinterpret_cast<sockaddr*>(&addr), addrLen);
		PlatformSocket::ThrowIfFailure(result);
		return static_cast<size_t>(result);
	}

	size_t Socket::ReceiveFrom(std::span<uint8> buffer, IPEndPoint& remoteEP)
	{
		EnsureSocket();
		sockaddr_storage addr;
		PlatformSocket::socket_len_type addrLen = sizeof(addr);

		int result = recvfrom(m_Socket->m_Socket, reinterpret_cast<char*>(buffer.data()),
			static_cast<int>(buffer.size()), 0, reinterpret_cast<sockaddr*>(&addr), &addrLen);
		PlatformSocket::ThrowIfFailure(result);
		remoteEP = SockAddrToIPEndPoint(addr);
		return static_cast<size_t>(result);
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
		sockaddr_storage addr;
		PlatformSocket::socket_len_type addrLen = sizeof(addr);
		int result = getsockname(m_Socket->m_Socket, reinterpret_cast<sockaddr*>(&addr), &addrLen);
		PlatformSocket::ThrowIfFailure(result);
		return SockAddrToIPEndPoint(addr);
	}

	IPEndPoint Socket::GetRemoteEndPoint() const
	{
		EnsureSocket();
		sockaddr_storage addr;
		PlatformSocket::socket_len_type addrLen = sizeof(addr);
		int result = getpeername(m_Socket->m_Socket, reinterpret_cast<sockaddr*>(&addr), &addrLen);
		PlatformSocket::ThrowIfFailure(result);
		return SockAddrToIPEndPoint(addr);
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
