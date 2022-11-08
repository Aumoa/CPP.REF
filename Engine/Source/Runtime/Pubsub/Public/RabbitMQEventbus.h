// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IPubsub.h"
#include "RabbitMQEventbus.gen.h"

class TcpConnectionHandler;

namespace AMQP
{
	class Connection;
}

SCLASS()
class PUBSUB_API RabbitMQEventbus : implements IPubsub
{
	GENERATED_BODY()

private:
	std::unique_ptr<TcpConnectionHandler> _connectionHandler;
	std::unique_ptr<AMQP::Connection> _connection;

public:
	RabbitMQEventbus();
	virtual ~RabbitMQEventbus() noexcept override;

	virtual void Publish(const String& topic, const String& payload) override;
	virtual void Subscribe(const String& topic, std::function<void(String)> func) override;
};