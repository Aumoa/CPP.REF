// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "ISqlDataParameters.h"

struct mysql_connection;
struct mysql_prepared_stmt;
class MySqlCommand;

class DATABASE_API MySqlDataParameters : public ISqlDataParameters
{
	std::shared_ptr<mysql_connection> _conn;
	std::shared_ptr<mysql_prepared_stmt> _stmt;

public:
	MySqlDataParameters(MySqlCommand* command);
	virtual ~MySqlDataParameters() noexcept override;

	virtual void Clear() override;
	virtual void Add(const DbParameter& item) override;
	virtual void RemoveAt(int32 index) override;
	virtual bool RemoveAt(String parameterName) override;
	virtual void Insert(int32 index, const DbParameter& item) override;

	virtual int32 IndexOf(const DbParameter& item) const noexcept override;
	virtual int32 IndexOf(String parameterName) const noexcept override;
};