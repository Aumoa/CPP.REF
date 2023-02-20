// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "DbType.h"

struct DbParameter
{
	EDbType DbType = EDbType::AnsiString;
	bool bIsNullable = false;
	String ParameterName;
	int32 Precision;
	std::variant<int64, uint64, String, DateTime, double> Value;
};