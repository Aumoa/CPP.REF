// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Net/NetEngine.h"
#include "Net/IPEndPoint.h"

NetEngine* NetEngine::Get()
{
	static NetEngine engine;
	return &engine;
}

void NetEngine::CreatePassiveSession()
{
	_passiveSession = Socket::NewTCPSocket();
	_passiveSession.Bind(IPEndPoint(IPAddress::Any(), 11000));
}