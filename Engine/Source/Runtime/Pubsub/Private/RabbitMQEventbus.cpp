// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RabbitMQEventbus.h"
#include "TcpConnectionHandler.h"
#include "amqpcpp.h"
#include "RabbitMQEventbus.gen.cpp"

RabbitMQEventbus::RabbitMQEventbus()
{
	_connectionHandler = std::make_unique<TcpConnectionHandler>();
	_connection = std::make_unique<AMQP::Connection>(_connectionHandler.get(), AMQP::Login("liberty", "Aumoa1367@"));
}

RabbitMQEventbus::~RabbitMQEventbus() noexcept
{
}

void RabbitMQEventbus::Publish(const String& topic, const String& payload)
{
}

void RabbitMQEventbus::Subscribe(const String& topic, std::function<void(String)> func)
{
}