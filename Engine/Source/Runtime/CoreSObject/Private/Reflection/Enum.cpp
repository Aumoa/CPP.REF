// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Reflection/Enum.h"
#include "CoreMinimal.h"

GENERATE_BODY(SEnum);

bool SEnum::TryParse(SType* type, std::wstring_view format, SObject*& result)
{
	if (!type->IsEnum())
	{
		result = nullptr;
		return false;
	}

	return type->_meta.EnumTryParseObj(format, result);
}

bool SEnum::TryParse(SType* type, std::wstring_view format, int64& result)
{
	if (!type->IsEnum())
	{
		result = 0;
		return false;
	}

	return type->_meta.EnumTryParse(format, result);
}