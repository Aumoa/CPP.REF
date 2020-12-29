// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "THAPI.h"
#include "GameMinimal.h"
#include "Level.h"

class TH_API StartupLevel : public Level
{
public:
	using Super = Level;
	using This = StartupLevel;

public:
	StartupLevel();
	~StartupLevel() override;

	void LoadLevel() override;
};