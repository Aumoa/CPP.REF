// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

interface IRHICommandList;

interface GAME_API IRHIBundle : virtual public Object
{
	virtual void InitializeBundle() = 0;
	virtual void ReleaseBundle() = 0;
	virtual void Commit(IRHICommandList* inCommandList) = 0;
};