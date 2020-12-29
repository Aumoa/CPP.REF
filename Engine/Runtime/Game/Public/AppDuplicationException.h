// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

class GAME_API AppDuplicationException : public Exception
{
public:
	using Super = Exception;
	using This = AppDuplicationException;

public:
	AppDuplicationException();
	~AppDuplicationException() override;
};