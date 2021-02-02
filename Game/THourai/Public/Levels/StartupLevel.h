// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "THAPI.h"
#include "GameMinimal.h"
#include "Level.h"

#include "Framework/SpectatorPawn.h"

class AMyTestActor;
class AMyCharacter;
class AStaticMeshActor;

class TH_API StartupLevel : public Level
{
public:
	using Super = Level;
	using This = StartupLevel;

private:
	AStaticMeshActor* plane;
	AStaticMeshActor* geosphere;
	AStaticMeshActor* teapot;
	AStaticMeshActor* cylinder;
	AStaticMeshActor* cone;
	AStaticMeshActor* icosahedron;

	ASpectatorPawn* spectator;

public:
	StartupLevel();
	~StartupLevel() override;

	void LoadLevel() override;

	APawn* GetPersistentActor() const;
};