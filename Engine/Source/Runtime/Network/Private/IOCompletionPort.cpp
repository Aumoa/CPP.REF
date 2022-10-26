// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "IOCompletionPort.h"
#include "Net/Socket.h"
#include "IOCompletionPort.gen.cpp"

static IOCompletionPort* sImpl;

IOCompletionPort::IOCompletionPort()
{
	check(sImpl == nullptr);
	sImpl = this;
}

IOCompletionPort::~IOCompletionPort() noexcept
{
	check(sImpl);
	sImpl = nullptr;
}

void IOCompletionPort::ChainSocket(ImplSocket* socket)
{
	HandleChainSocket(socket);
}

IOCompletionPort* IOCompletionPort::Get()
{
	return sImpl;
}