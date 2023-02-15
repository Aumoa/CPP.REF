// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "MySql/MySqlCommand.h"
#include "MySql/MySqlConnection.h"
#include "MySql/mysql_connection.h"
#include "MySql/mysql_prepared_stmt.h"

MySqlCommand::MySqlCommand(String sql, MySqlConnection* sqlConnection)
	: MySqlCommand(sqlConnection)
{
	_sql = sql;
	Prepare();
}

MySqlCommand::MySqlCommand(MySqlConnection* sqlConnection)
	: _stmt(std::make_unique<mysql_prepared_stmt>())
{
	_conn = sqlConnection->_conn;
}

MySqlCommand::~MySqlCommand() noexcept
{
}

void MySqlCommand::Prepare()
{
	if (_stmt->stmt)
	{
		_stmt->close();
	}

	_stmt->stmt = _conn->connection->prepareStatement(sql::SQLString(_sql.AsCodepage(65001)));
}

void MySqlCommand::SetCommandText(String sql)
{
	if (_sql != sql)
	{
		if (_stmt->stmt)
		{
			_stmt->close();
		}

		_sql = sql;
	}
}

String MySqlCommand::GetCommandText() const
{
	return _sql;
}