// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Sessions/ClientSession.h"
#include "Net/Socket.h"
#include "Net/Packet.h"

ClientSession::ClientSession(std::unique_ptr<Socket> sock, int64 sessionId)
	: _sock(std::move(sock))
	, _sessionId(sessionId)
{
	SessionDisconnected.AddLambda([](ClientSession* session)
	{
		session->CloseSession();
	});
}

ClientSession::~ClientSession() noexcept
{
	if (ensure(_sock == nullptr) == false)
	{
		CloseSession();
	}
}

void ClientSession::Start()
{
	StartReceiver();
}

void ClientSession::CloseSession() noexcept
{
	_ss.request_stop();

	if (_sock)
	{
		_sock->Close();
		_sock.reset();
	}
}

Task<> ClientSession::SendPacket(std::shared_ptr<Packet> p, std::stop_token cancellationToken)
{
	SocketBuffer buffer = SocketBuffer::CopyReadonly(p->GetBuffer());
	size_t sent = 0;

	while (sent < buffer.GetBufferSize())
	{
		sent += co_await _sock->SendAsync(buffer.Subbuffer(sent), cancellationToken);
	}
}

void ClientSession::StartReceiver()
{
	SocketBuffer buf = SocketBuffer::Alloc(1024);
	auto builder = Packet::CreateBuilder();

	try
	{
		while (!_ss.stop_requested())
		{
			size_t bytesToRead = co_await _sock->ReceiveAsync(buf, _ss.get_token());
			if (bytesToRead == 0)
			{
				CloseSession();
				SessionDisconnected.Broadcast(this);
				co_return;
			}

			size_t reads = 0;
			while (bytesToRead > reads)
			{
				reads += builder.Append(buf.AsReadonlySpan(reads, bytesToRead - reads));
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
	catch (const TaskCanceledException&)
	{
		SessionDisconnected.Broadcast(this);
	}
}