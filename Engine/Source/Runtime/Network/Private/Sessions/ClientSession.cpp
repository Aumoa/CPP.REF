// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Sessions/ClientSession.h"
#include "Net/Socket.h"
#include "Net/Packet.h"

ClientSession::ClientSession(std::unique_ptr<Socket> sock, int64 sessionId)
	: _sock(std::move(sock))
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

Task<> ClientSession::SendPacket(std::shared_ptr<Packet> p, std::stop_token cancellationToken)
{
	auto buffer = p->GetBuffer();
	size_t sent = 0;

	while (sent >= buffer.size_bytes())
	{
		sent += co_await _sock->SendAsync(buffer.subspan(sent), cancellationToken);
	}
}

void ClientSession::StartReceiver()
{
	std::vector<uint8> buffer(1024);
	auto builder = Packet::CreateBuilder();

	try
	{
		while (!_ss.stop_requested())
		{
			size_t bytesToRead = co_await _sock->ReceiveAsync(std::span(buffer), _ss.get_token());
			if (bytesToRead == 0)
			{
				SessionDisconnected.Broadcast(this);
				_sock->Close();
				co_return;
			}

			size_t reads = 0;
			while (bytesToRead > reads)
			{
				reads += builder.Append(std::span(buffer.data() + reads, buffer.data() + bytesToRead));
				if (builder.IsCompleted())
				{
					auto p = builder.Build();
					builder = Packet::CreateBuilder();
					MessageReceived.Broadcast(this, std::move(p));
				}
			}
		}
	}
	catch (const SocketException& se)
	{
		bool bDisconnect = true;
		SocketError.Broadcast(this, std::ref(se), std::ref(bDisconnect));
		if (bDisconnect == false)
		{
			// Start new.
			StartReceiver();
			co_return;
		}
		SessionDisconnected.Broadcast(this);
	}
}