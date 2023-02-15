// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class MySqlCommand;
struct mysql_connection;

class DATABASE_API MySqlConnection
{
	friend MySqlCommand;
	std::shared_ptr<mysql_connection> _conn;

public:
	MySqlConnection(String connectionString);
	~MySqlConnection() noexcept;

	void Open();
	void Close() noexcept;
	MySqlCommand CreateCommand();

private:
	static void ParseConnectionStringInto(String connectionString, void* outOptions);
};