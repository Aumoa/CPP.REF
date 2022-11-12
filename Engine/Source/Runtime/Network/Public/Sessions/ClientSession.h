// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Net/Socket.h"
#include "ClientSession.gen.h"

class Packet;
class SocketException;

SCLASS()
class NETWORK_API ClientSession : implements Object
{
	GENERATED_BODY()

private:
	Socket _sock;
	int64 _sessionId;
	std::stop_source _ss;

public:
	ClientSession(Socket sock, int64 sessionId);
	virtual ~ClientSession() noexcept override;

	void Start();
	void CloseSession() noexcept;
	void SendPacket(std::shared_ptr<Packet> p);

	inline int64 GetSessionId() noexcept { return _sessionId; }

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