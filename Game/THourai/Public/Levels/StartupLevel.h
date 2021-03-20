// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "THAPI.h"
#include "GameMinimal.h"
#include "Level.h"

#include "Framework/SpectatorPawn.h"

class AMyTestActor;
class AMyCharacter;
class AStaticMeshActor;
class ADirectionalLight;
class APointLight;
class ARotateLight;
class ASpotLight;

class TH_API GStartupLevel : public GLevel
{
public:
	using Super = GLevel;
	using This = GStartupLevel;

private:
	ADirectionalLight* light;
	APointLight* rotateLight;
	ASpotLight* spotLight;

	AStaticMeshActor* plane;
	AStaticMeshActor* geosphere;
	AStaticMeshActor* teapot;
	AStaticMeshActor* cylinder;
	AStaticMeshActor* cone;
	AStaticMeshActor* icosahedron;

	ASpectatorPawn* spectator;

public:
	GStartupLevel();
	~GStartupLevel() override;

	void LoadLevel() override;

	APawn* GetPersistentActor() const;
};