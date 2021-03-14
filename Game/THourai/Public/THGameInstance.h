// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "THAPI.h"
#include "GameMinimal.h"
#include "GameInstance.h"

class GStartupLevel;
class TickScheduler;

class TH_API THGameInstance : public GameInstance
{
public:
	using Super = GameInstance;
	using This = THGameInstance;

private:
	TRefPtr<TickScheduler> diag_tick;

public:
	THGameInstance();
	~THGameInstance();
};