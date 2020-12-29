// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "THAPI.h"
#include "GameMinimal.h"
#include "GameInstance.h"

class StartupLevel;

class TH_API THGameInstance : public GameInstance
{
public:
	using Super = GameInstance;
	using This = THGameInstance;

private:
	TRefPtr<StartupLevel> startupLevel;

public:
	THGameInstance();
	~THGameInstance();

	TRefPtr<String> ToString() const;
	Level* GetStartupLevel() override;
};