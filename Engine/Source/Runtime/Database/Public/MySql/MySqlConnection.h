// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IDbConnection.h"

class DATABASE_API MySqlConnection : public IDbConnection
{
	struct connection;
	std::unique_ptr<connection> _conn;

public:
	MySqlConnection(String connectionString);
	virtual ~MySqlConnection() noexcept override;

	virtual void Open() override;
	virtual void Close() noexcept override;
	virtual std::shared_ptr<IDbCommand> CreateCommand() override;

private:
	static void ParseConnectionStringInto(String connectionString, void* outOptions);
};