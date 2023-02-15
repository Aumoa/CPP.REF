// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "MySql/MySqlConnection.h"
#include "MySql/MySqlCommand.h"
#include "MySql/mysql_connection.h"

MySqlConnection::MySqlConnection(String connectionString)
{
	_conn = std::make_shared<mysql_connection>();
	ParseConnectionStringInto(connectionString, &_conn->options);
}

MySqlConnection::~MySqlConnection() noexcept
{
}

void MySqlConnection::Open()
{
	if (_conn->connection)
	{
		throw InvalidOperationException(TEXT("IDbConnection already opened."));
	}

	auto driver = get_driver_instance();
	_conn->connection = driver->connect(_conn->options);
}

void MySqlConnection::Close() noexcept
{
	_conn->close();
}

MySqlCommand MySqlConnection::CreateCommand()
{
	return MySqlCommand(this);
}

void MySqlConnection::ParseConnectionStringInto(String connectionString, void* outOptions)
{
	auto& options = *reinterpret_cast<sql::ConnectOptionsMap*>(outOptions);
	std::vector<String> splits = connectionString.Split(TEXT(";"), EStringSplitOptions::RemoveEmptyEntries | EStringSplitOptions::TrimEntries);
	for (auto& entry : splits)
	{
		std::vector<String> keyValuePair = entry.Split(TEXT("="));
		if (keyValuePair.size() != 2)
		{
			throw InvalidOperationException(TEXT("Connection string is not valid."));
		}

		sql::SQLString key = keyValuePair[0].AsCodepage(65001);
		sql::SQLString value = keyValuePair[1].AsCodepage(65001);
		options.emplace(key, value);
	}
}