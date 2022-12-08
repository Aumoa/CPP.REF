// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Net/Socket.h"

class Packet;
class SocketException;

class NETWORK_API ClientSession : public EnableSharedFromThis<ClientSession>
{
	using This = ClientSession;

private:
	std::unique_ptr<Socket> _sock;
	int64 _sessionId;
	std::stop_source _ss;
	TaskCompletionSource<> _close;

public:
	ClientSession(std::unique_ptr<Socket> sock, int64 sessionId);
	virtual ~ClientSession() noexcept;

	void Start();
	void CloseSession() noexcept;
	Task<> SendPacket(std::shared_ptr<Packet> p, std::stop_token cancellationToken = {});

	inline int64 GetSessionId() const noexcept { return _sessionId; }
	inline const IPEndPoint& GetRemoteEndPoint() const noexcept { return _sock->GetRemoteEndPoint(); }

public:
	DECLARE_MULTICAST_EVENT(MessageReceivedEvent, ClientSession*, std::shared_ptr<Packet>);
	MessageReceivedEvent MessageReceived;
	DECLARE_MULTICAST_EVENT(SessionEvent, ClientSession*);
	SessionEvent SessionDisconnected;
	DECLARE_MULTICAST_EVENT(SocketErrorEvent, ClientSession*, const SocketException&, bool&);
	SocketErrorEvent SocketError;

private:
	void StartReceiver();
};