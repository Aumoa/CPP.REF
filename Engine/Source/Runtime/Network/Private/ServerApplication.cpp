// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "ServerApplication.h"
#include "ServiceSession.h"
#include "ClientSession.h"
#include "Net/Socket.h"
#include "ServerApplication.gen.cpp"

ServerApplication::ServerApplication()
{
}

ServerApplication::~ServerApplication() noexcept
{
}

void ServerApplication::Start()
{
	_server = std::make_unique<ServiceSession>();
	_server->SocketConnected.AddObject(this, &ServerApplication::OnSocketConnected);
	_server->Start();
}

void ServerApplication::Update(const TimeSpan& deltaTime)
{
	std::unique_lock lock(_lock);
	std::swap(_pendingSockets, _pendingSockets_buf);
	lock.unlock();

	for (auto& sock : _pendingSockets_buf)
	{
		int64 id = 0;
		if (_sessionIdPool.size() > 0)
		{
			id = _sessionIdPool.back();
			_sessionIdPool.erase(--_sessionIdPool.end());
		}
		else
		{
			id = (int64)_sessions.size();
		}

		auto& session = _sessions.emplace(id, std::make_unique<ClientSession>(sock, id)).first->second;
		SessionConnected.Broadcast(session.get());

		session->Start();
	}

	_pendingSockets_buf.clear();
}

void ServerApplication::OnApplicationShutdown() noexcept
{
	if (_server)
	{
		_server->CloseSession();
		_server = nullptr;
	}
}

void ServerApplication::OnSocketConnected(Socket sock)
{
	std::unique_lock lock(_lock);
	_pendingSockets.emplace_back(sock);
}