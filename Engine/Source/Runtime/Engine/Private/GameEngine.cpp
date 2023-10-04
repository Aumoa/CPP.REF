// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "GameEngine.h"
#include "Type.h"
#include "Package.h"
#include "EngineSubsystem.h"
#include "Actors/GameInstance.h"
#include "World.h"

AGameEngine* GEngine = nullptr;

AGameEngine::AGameEngine()
{
	GEngine = this;
}

AGameEngine::~AGameEngine() noexcept
{
}

void AGameEngine::Initialize()
{
	SpawnWorld();
	InitSubsystems();

	GameWorld->BeginPlay();
}

void AGameEngine::Deinitialize()
{
	GameWorld->EndPlay();

	Subsystems.Deinitialize();
}

void AGameEngine::SpawnWorld()
{
	GameWorld = NewObject<AWorld>();
	auto* Instance = static_cast<AGameInstance*>(GameWorld->SpawnPersistentActor(AGameInstance::StaticClass()));
	Instance->Initialize();
}

void AGameEngine::InitSubsystems()
{
	AddSubsystemsForPackage(AGameEngine::StaticClass()->GetPackage());
	AddSubsystemsForPackage(GetType()->GetPackage());

	Subsystems.Initialize();
	Subsystems.PostInitialize();
}

void AGameEngine::AddSubsystemsForPackage(APackage* InPackage)
{
	for (AType* PkgType : InPackage->GetTypes())
	{
		if (PkgType->IsDerivedFrom<AEngineSubsystem>())
		{
			Subsystems.Add(PkgType);
		}
	}
}