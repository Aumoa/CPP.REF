// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "TcpConnectionHandler.h"
#include "Exceptions/NotImplementedException.h"

TcpConnectionHandler::TcpConnectionHandler()
{
}

void TcpConnectionHandler::onData(AMQP::Connection* connection, const char* buffer, size_t size)
{
	throw NotImplementedException();
}