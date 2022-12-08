// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Sessions/ServiceSession.h"
#include "Sessions/ClientSession.h"
#include "Services/IServiceProvider.h"

constexpr static LogCategory LogServer(TEXT("LogServer"));

ServiceSession::ServiceSession(IServiceProvider* sp)
{
	_sock = std::make_unique<Socket>(EAddressFamily::InterNetwork, ESocketType::Stream, EProtocolType::Tcp);
}

ServiceSession::~ServiceSession() noexcept
{
}

Task<> ServiceSession::StartAsync(std::stop_token cancellationToken)
{
	return Task<>::CompletedTask();
}

Task<> ServiceSession::StopAsync(std::stop_token cancellationToken)
{
	try
	{
		_ss.request_stop();
		co_await _acceptor;
	}
	catch (const TaskCanceledException&)
	{
	}

	_sock->Close();
}

Task<> ServiceSession::StartService(int32 servicePort)
{
	auto init = TaskCompletionSource<>::Create();

	IPEndPoint ep = IPEndPoint(IPAddress::Any(), (uint16)servicePort);
	_sock->Bind(ep);
	_acceptor = StartSocketAcceptor(_ss.get_token(), init);
	co_await init.GetTask();

	Log::Info(LogServer, TEXT("Service acceptor started with endpoint: {}"), ep);
}

Task<> ServiceSession::StartSocketAcceptor(std::stop_token cancellationToken, TaskCompletionSource<> init)
{
	_sock->Listen();
	init.SetResult();

	int64 sessionId = 0;
	while (!cancellationToken.stop_requested())
	{
		auto client = std::make_unique<Socket>(EAddressFamily::InterNetwork, ESocketType::Stream, EProtocolType::Tcp);
		co_await _sock->AcceptAsync(*client, cancellationToken);

		Log::Info(LogServer, TEXT("Client {} connected."), client->GetRemoteEndPoint());
		auto session = std::make_unique<ClientSession>(std::move(client), ++sessionId);
		OnSessionConnected(std::move(session));
	}
}