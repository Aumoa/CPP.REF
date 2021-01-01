// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

interface GAME_API IRHIBundle : virtual public Object
{
	virtual void InitializeBundle() = 0;
	virtual void ReleaseBundle() = 0;
};