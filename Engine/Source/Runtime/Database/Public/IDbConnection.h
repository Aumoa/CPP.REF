// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class IDbCommand;

class IDbConnection
{
public:
	virtual ~IDbConnection() noexcept = default;

	virtual void Open() = 0;
	virtual void Close() noexcept = 0;
	virtual std::shared_ptr<IDbCommand> CreateCommand() = 0;
};