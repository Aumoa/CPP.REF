// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Sessions/ClientSession.h"
#include "Net/Socket.h"
#include "Net/Packet.h"
#include "ClientSession.gen.cpp"

ClientSession::ClientSession(Socket sock, int64 sessionId)
	: _sock(sock)
	, _sessionId(sessionId)
{
}

ClientSession::~ClientSession() noexcept
{
}

void ClientSession::Start()
{
	StartReceiver();
}

void ClientSession::CloseSession() noexcept
{
	_ss.request_stop();
}

void ClientSession::SendPacket(std::shared_ptr<Packet> p)
{
	co_await _sock.SendAsync(p->GetBuffer());
}

void ClientSession::StartReceiver()
{
	std::vector<uint8> buffer(1024);
	auto builder = Packet::CreateBuilder();
	size_t reads = 0;

	while (!_ss.stop_requested())
	{
		size_t bytesToRead = co_await _sock.ReceiveAsync(buffer, _ss.get_token());
		if (bytesToRead == 0)
		{
			SessionDisconnected.Broadcast(this);
			_sock.Close();
			co_return;
		}

		while (bytesToRead > reads)
		{
			reads += builder.Append(std::span(buffer.data(), buffer.data() + bytesToRead));
			if (builder.IsCompleted())
			{
				auto p = builder.Build();
				builder = Packet::CreateBuilder();
				MessageReceived.Broadcast(this, std::move(p));
			}
		}
	}
}