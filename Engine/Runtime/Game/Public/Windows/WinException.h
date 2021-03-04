// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

class GAME_API WinException : public Exception
{
public:
	using Super = Exception;
	using This = WinException;

private:
	int32 lastError;

public:
	WinException();
	WinException(int32 lastError);
	~WinException() override;

	vs_property_get_auto(int32, Code, lastError);

private:
	static TRefPtr<String> GetErrorMessage(int32 lastError);
};