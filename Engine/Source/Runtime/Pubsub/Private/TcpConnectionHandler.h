// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "amqpcpp.h"

class TcpConnectionHandler : public AMQP::ConnectionHandler
{
public:
	TcpConnectionHandler();

	virtual void onData(AMQP::Connection* connection, const char* buffer, size_t size) override;
};