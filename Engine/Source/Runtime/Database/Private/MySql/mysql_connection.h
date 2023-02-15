// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "MySql/mysql.h"

struct mysql_connection
{
	sql::ConnectOptionsMap options;
	sql::Connection* connection = nullptr;

	mysql_connection() noexcept
	{
	}

	~mysql_connection() noexcept
	{
		if (connection)
		{
			close();
		}
	}

	void connect()
	{
		auto driver = get_driver_instance();
		connection = driver->connect(options);
	}

	void close() noexcept
	{
		check(connection);
		connection->close();
		delete std::exchange(connection, nullptr);
	}
};