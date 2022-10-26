// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Net/Socket.h"

#if PLATFORM_WINDOWS
#include "Windows/WinSocket.h"
#endif

Socket::Socket()
{
}

Socket::Socket(const Socket& rhs)
	: _impl(rhs._impl)
{
}

Socket::~Socket() noexcept
{
}

void Socket::Bind(const IPEndPoint& ep)
{
	_impl->Bind(ep);
}
 
Task<> Socket::ConnectAsync(const IPEndPoint& ep)
{
	return _impl->ConnectAsync(ep);
}

void Socket::Listen()
{
	_impl->Listen();
}

Task<Socket> Socket::AcceptAsync()
{
	Socket s;
	s._impl = co_await _impl->AcceptAsync();
	co_return s;
}

Task<size_t> Socket::ReceiveAsync(void* bufferToRecv, size_t len, std::stop_token cancellationToken)
{
	return _impl->ReceiveAsync(bufferToRecv, len, cancellationToken);
}

Task<size_t> Socket::SendAsync(const void* bufferToSend, size_t len, std::stop_token cancellationToken)
{
	return _impl->SendAsync(bufferToSend, len, cancellationToken);
}

void Socket::Close()
{
	_impl->Close();
}

Socket Socket::CreateTCP()
{
	Socket sock;
#if PLATFORM_WINDOWS
	sock._impl = std::make_unique<WinSocket>();
#endif
	return sock;
}