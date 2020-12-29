// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

struct LogCategoryBase;

enum class TickingGroup
{
	PrePhysics,
	DuringPhysics,
	PostPhysics,
	PostUpdateWork
};

struct GAME_API TickFunction
{
	static LogCategoryBase LogTicking;

	bool bCanEverTick : 1;
	TickingGroup TickGroup;
	std::chrono::duration<double> TickInterval;

	TickFunction();
	~TickFunction();

	virtual void ExecuteTick(std::chrono::duration<double> deltaTime) = 0;
};