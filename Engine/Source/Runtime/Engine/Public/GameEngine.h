// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Object.h"
#include "SubclassOf.h"
#include "SubsystemCollection.h"
#include "EngineSubsystem.h"
#include "GameEngine.generated.h"

class AEngineSubsystem;
class APackage;
class AWorld;
class SViewport;

ACLASS()
class ENGINE_API AGameEngine : public AObject
{
	GENERATED_BODY()

private:
	NSubsystemCollection Subsystems;
	RefPtr<AWorld> GameWorld;

public:
	AGameEngine();
	virtual ~AGameEngine() noexcept override;

	virtual void Initialize();
	virtual void Deinitialize();

	NSubsystemCollection& GetEngineSubsystems() { return Subsystems; }

protected:
	void SpawnWorld();
	virtual void InitSubsystems();

private:
	void AddSubsystemsForPackage(APackage* InPackage);
};

extern ENGINE_API AGameEngine* GEngine;