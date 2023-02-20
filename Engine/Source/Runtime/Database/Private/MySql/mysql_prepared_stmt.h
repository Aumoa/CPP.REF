// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "MySql/mysql.h"

struct mysql_prepared_stmt
{
	sql::PreparedStatement* stmt = nullptr;

	mysql_prepared_stmt() noexcept
	{
	}

	~mysql_prepared_stmt() noexcept
	{
		if (stmt)
		{
			close();
		}
	}

	bool is_prepared() const noexcept
	{
		return stmt != nullptr;
	}

	void close() noexcept
	{
		check(stmt);
		delete std::exchange(stmt, nullptr);
	}
};