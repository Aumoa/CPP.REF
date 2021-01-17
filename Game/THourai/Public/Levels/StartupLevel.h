// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "THAPI.h"
#include "GameMinimal.h"
#include "Level.h"

class AMyTestActor;
class AMyCharacter;

class TH_API StartupLevel : public Level
{
public:
	using Super = Level;
	using This = StartupLevel;

private:
	AMyTestActor* persistent_actor;
	AMyCharacter* persistent_character;

public:
	StartupLevel();
	~StartupLevel() override;

	void LoadLevel() override;

	AMyCharacter* GetPersistentActor() const;
};