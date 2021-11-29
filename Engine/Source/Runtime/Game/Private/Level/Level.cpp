// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Level/Level.h"
#include "Level/World.h"
#include "LogGame.h"
#include "GameFramework/PlayerController.h"
#include "Info/GameMode.h"
#include "Ticking/TickTaskLevelManager.h"

SLevel::SLevel() : Super()
	, GameModeClass(AGameMode::StaticClass())
{
}

SLevel::~SLevel()
{
}

bool SLevel::LoadLevel(SWorld* InWorld, STickTaskLevelManager* InParentLevelTick)
{
	checkf(GameModeClass.IsValid(), L"GameModeClass does not specified.");

	_World = InWorld;

	_GameMode = Cast<AGameMode>(SpawnActor(GameModeClass));
	_PlayerController = _GameMode->SpawnPlayerController();

	_LevelTick = InParentLevelTick;
	if (_LevelTick == nullptr)
	{
		_LevelTick = NewObject<STickTaskLevelManager>();
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

	AActor* Actor = InActorClass.Instantiate(this);
	if (Actor == nullptr)
	{
		SE_LOG(LogWorld, Error, L"Actor class does not support instantiate without any constructor arguments.");
		return nullptr;
	}

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
	return _PlayerController;
}

SWorld* SLevel::GetWorld()
{
	return _World;
}

STickTaskLevelManager* SLevel::GetLevelTick()
{
	return _LevelTick;
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
		_LevelTick->RemoveTickFunction(&InActor->PrimaryActorTick);
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
		_LevelTick->AddTickFunction(&InActor->PrimaryActorTick);
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