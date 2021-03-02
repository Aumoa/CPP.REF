// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

class GAME_API COMException : public Exception
{
public:
	using Super = Exception;

public:
	COMException(int32 errorCode);
	~COMException() override;

	static bool Succeeded(int32 errorCode);
	static bool Failed(int32 errorCode);

private:
	static TRefPtr<String> GetHResultString(int32 hr);
};