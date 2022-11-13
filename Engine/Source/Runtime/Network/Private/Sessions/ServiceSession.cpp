// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Sessions/ServiceSession.h"
//#include "Net/Socket.h"
//#include "Net/SocketException.h"
//#include "Threading/Thread.h"
//#include "Diagnostics/PerformanceTimer.h"
//#include "ServiceSession.gen.cpp"
//
//constexpr static LogCategory LogServer(TEXT("LogServer"));
//
//ServiceSession::ServiceSession()
//{
//	_sock = Socket::CreateTCP();
//}
//
//ServiceSession::~ServiceSession() noexcept
//{
//	CloseSession();
//}
//
//void ServiceSession::Start()
//{
//	_sock.Bind(IPEndPoint{ .Address = IPAddress::Any(), .Port = 11000 });
//	_sock.Listen();
//
//	_acceptor = StartSocketAcceptor(_ss.get_token());
//}
//
//void ServiceSession::CloseSession() noexcept
//{
//	_ss.request_stop();
//	_sock.Close();
//}
//
//Task<> ServiceSession::StartSocketAcceptor(std::stop_token cancellationToken)
//{
//	co_yield co_push(cancellationToken);
//
//	while (!cancellationToken.stop_requested())
//	{
//		Socket sock = co_await _sock.AcceptAsync();
//		SocketConnected.Broadcast(sock);
//	}
//}