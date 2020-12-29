// Copyright 2020 Aumoa.lib. All right reserved.

#include "THAPI.h"
#include "Level.h"
#include "Framework/StaticMeshActor.h"

export module StartupLevel;

using namespace SC::Runtime::Core;
using namespace SC::Runtime::Game;
using namespace SC::Runtime::Game::Framework;

export class TH_API StartupLevel : public Level
{
public:
	using Super = Level;
	using This = StartupLevel;

public:
	StartupLevel()
	{

	}

	~StartupLevel() override
	{

	}

	void LoadLevel() override
	{
		SpawnActorPersistent<AStaticMeshActor>();
	}
};