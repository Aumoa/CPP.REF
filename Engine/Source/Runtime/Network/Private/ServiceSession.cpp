// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "ServiceSession.h"
#include "Net/Socket.h"
#include "Net/SocketException.h"
#include "Threading/Thread.h"
#include "Diagnostics/PerformanceTimer.h"
#include "ServiceSession.gen.cpp"

constexpr static LogCategory LogServer(TEXT("LogServer"));

ServiceSession::ServiceSession()
{
	_sock = std::make_unique<Socket>(EAddressFamily::InterNetwork, ESocketType::Stream, EProtocolType::Tcp);
}

ServiceSession::~ServiceSession() noexcept
{
	CloseSession();
}

void ServiceSession::Start()
{
	_sock->Bind(EndPoint{ .Address = IPAddress::Any(), .Port = 11000 });
	_sock->Listen();

	_acceptor = Thread::CreateThread(TEXT("[ServerAcceptor]"), std::bind(&ServiceSession::StartSocketAcceptor, this, _ss.get_token()));
}

void ServiceSession::CloseSession() noexcept
{
	_ss.request_stop();
	_sock.reset();
}

void ServiceSession::StartSocketAcceptor(std::stop_token cancellationToken)
{
	while (!cancellationToken.stop_requested())
	{
		std::unique_ptr<Socket> sock = _sock->Accept();
		SocketConnected.Broadcast(sock.release());
	}
}