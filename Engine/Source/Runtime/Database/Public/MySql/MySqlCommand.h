// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "MySql/MySqlConnection.h"

struct mysql_connection;
struct mysql_prepared_stmt;

class DATABASE_API MySqlCommand
{
	std::shared_ptr<mysql_connection> _conn;
	std::shared_ptr<mysql_prepared_stmt> _stmt;
	String _sql;

public:
	MySqlCommand(String sql, MySqlConnection* sqlConnection);
	MySqlCommand(MySqlConnection* sqlConnection);
	~MySqlCommand() noexcept;

	void Prepare();
	void SetCommandText(String sql);
	String GetCommandText() const;
};