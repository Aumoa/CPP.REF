// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IDbCommand.h"

class MySqlConnection;

class DATABASE_API MySqlCommand : public IDbCommand
{
	PolPtr<MySqlConnection> _connection;

public:
	MySqlCommand(PolPtr<MySqlConnection> sqlConnection);
	virtual ~MySqlCommand() noexcept override;
};