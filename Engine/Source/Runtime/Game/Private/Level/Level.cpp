// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Level/Level.h"
#include "Level/World.h"
#include "LogGame.h"
#include "GameFramework/PlayerController.h"
#include "Info/GameMode.h"
#include "Ticking/TickTaskLevelManager.h"

GENERATE_BODY(SLevel);

SLevel::SLevel() : Super()
	, GameModeClass(AGameMode::StaticClass())
{
}

bool SLevel::LoadLevel(SWorld* InWorld, STickTaskLevelManager* InParentLevelTick)
{
	checkf(GameModeClass.IsValid(), L"GameModeClass does not specified.");

	World = InWorld;

	GameMode = Cast<AGameMode>(SpawnActor(GameModeClass));
	PlayerController = GameMode->SpawnPlayerController();

	LevelTick = InParentLevelTick;
	if (LevelTick == nullptr)
	{
		LevelTick = gcnew STickTaskLevelManager(World);
	}

	OnLoadLevel();
	return true;
}

void SLevel::UnloadLevel()
{
	IncrementalActorsApply(0);
	OnUnloadLevel();

	for (auto Actor : Actors)
	{
		InternalRemoveActor(Actor);
	}
	Actors.clear();
}

AActor* SLevel::SpawnActor(SubclassOf<AActor> InActorClass, bool bSpawnIncremental)
{
	if (!InActorClass.IsValid())
	{
		SE_LOG(LogWorld, Error, L"Actor class does not specified. Abort.");
		return nullptr;
	}

	AActor* Actor = Cast<AActor>(InActorClass->Instantiate());
	if (Actor == nullptr)
	{
		SE_LOG(LogWorld, Error, L"Actor class does not support instantiate without any constructor arguments.");
		return nullptr;
	}

	Actor->SetOuter(this);
	if (bSpawnIncremental)
	{
		ActorsToAdd.emplace_back(Actor);
	}
	else
	{
		InternalAddActor(Actor);
	}

	return Actor;
}

void SLevel::DestroyActor(AActor* InActor)
{
	ActorsToRemove.emplace_back(InActor);
}

void SLevel::IncrementalActorsApply(size_t InLimit)
{
	if (InLimit == 0)
	{
		InLimit = std::numeric_limits<size_t>::max();
	}

	// Remove actors first.
	auto EndIt = ActorsToRemove.rbegin();
	for (; InLimit-- && EndIt != ActorsToRemove.rend(); ++EndIt)
	{
		InternalRemoveActor(*EndIt);
	}
	ActorsToRemove.erase(EndIt.base(), ActorsToRemove.end());

	EndIt = ActorsToAdd.rbegin();
	for (; InLimit-- && EndIt != ActorsToAdd.rend(); ++EndIt)
	{
		InternalAddActor(*EndIt);
	}
	ActorsToAdd.erase(EndIt.base(), ActorsToAdd.end());
}

APlayerController* SLevel::GetPlayerController()
{
	return PlayerController;
}

SWorld* SLevel::GetWorld()
{
	return World;
}

STickTaskLevelManager* SLevel::GetLevelTick()
{
	return LevelTick;
}

void SLevel::InternalRemoveActor(AActor* InActor, bool bRemoveFromArray)
{
	InActor->DispatchEndPlay();

	for (auto SceneComponent : InActor->GetSceneComponents())
	{
		if (SceneComponent->IsRegistered())
		{
			SceneComponent->UnregisterComponent();
		}
	}

	for (auto ActorComponent : InActor->GetOwnedComponents())
	{
		if (ActorComponent->IsRegistered())
		{
			ActorComponent->UnregisterComponent();
		}
	}

	if (InActor->PrimaryActorTick.IsTickFunctionRegistered())
	{
		LevelTick->RemoveTickFunction(&InActor->PrimaryActorTick);
	}

	if (bRemoveFromArray)
	{
		auto It = std::find(Actors.begin(), Actors.end(), InActor);
		Actors.erase(It);
	}
}

void SLevel::InternalAddActor(AActor* InActor)
{
	Actors.emplace_back(InActor);

	if (!InActor->PrimaryActorTick.IsTickFunctionRegistered())
	{
		LevelTick->AddTickFunction(&InActor->PrimaryActorTick);
	}

	for (auto ActorComponent : InActor->GetOwnedComponents())
	{
		if (!ActorComponent->IsRegistered())
		{
			ActorComponent->RegisterComponentWithWorld(GetWorld());
		}
	}

	for (auto SceneComponent : InActor->GetSceneComponents())
	{
		if (!SceneComponent->IsRegistered())
		{
			SceneComponent->RegisterComponentWithWorld(GetWorld());
		}
	}

	InActor->PostInitializedComponents();
	InActor->DispatchBeginPlay();
}