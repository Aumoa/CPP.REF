// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Level/World.h"
#include "Level/Level.h"
#include "GameEngine.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/PlayerController.h"
#include "SceneRendering/Scene.h"
#include "Scene/PrimitiveSceneProxy.h"
#include "Camera/PlayerCameraManager.h"
#include "EngineSubsystems/GameRenderSystem.h"

template<ETickingGroup _Group>
bool SWorld::TickGroup<_Group>::Add(STickFunction* function)
{
	check(function->TickGroup == Group);
	return Functions.emplace(function).second;
}

template<ETickingGroup _Group>
bool SWorld::TickGroup<_Group>::Remove(STickFunction* function)
{
	check(function->TickGroup == Group);
	return Functions.erase(function) > 0;
}

template<ETickingGroup _Group>
void SWorld::TickGroup<_Group>::ReadyForExecuteTick()
{
	for (auto& function : Functions) { function->Ready(); }
}

template<ETickingGroup _Group>
void SWorld::TickGroup<_Group>::ExecuteTick(float elapsedTime)
{
	for (auto& function : Functions) { function->ExecuteTick(elapsedTime); }
}

bool SWorld::TickFunctions::Add(STickFunction* function)
{
	switch (function->TickGroup)
	{
	case ETickingGroup::PrePhysics: return PrePhysics.Add(function);
	case ETickingGroup::DuringPhysics: return DuringPhysics.Add(function);
	case ETickingGroup::PostPhysics: return PostPhysics.Add(function);
	case ETickingGroup::PostUpdateWork: return PostUpdateWork.Add(function);
	}
	return ensure(false);
}

bool SWorld::TickFunctions::Remove(STickFunction* function)
{
	switch (function->TickGroup)
	{
	case ETickingGroup::PrePhysics: return PrePhysics.Remove(function);
	case ETickingGroup::DuringPhysics: return DuringPhysics.Remove(function);
	case ETickingGroup::PostPhysics: return PostPhysics.Remove(function);
	case ETickingGroup::PostUpdateWork: return PostUpdateWork.Remove(function);
	}
	return ensure(false);
}

void SWorld::TickFunctions::ReadyForExecuteTick()
{
	PrePhysics.ReadyForExecuteTick();
	DuringPhysics.ReadyForExecuteTick();
	PostPhysics.ReadyForExecuteTick();
	PostUpdateWork.ReadyForExecuteTick();
}

SWorld::SWorld() : Super()
{
}

SWorld* SWorld::GetWorld()
{
	return this;
}

AActor* SWorld::SpawnActor(SubclassOf<AActor> InActorClass)
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

	// Register actor tick function.
	RegisterTickFunction(&Actor->PrimaryActorTick);

	for (auto& OwnedComponent : Actor->GetOwnedComponents())
	{
		if (!OwnedComponent->IsRegistered())
		{
			OwnedComponent->RegisterComponentWithWorld(this);
		}
	}

	for (auto& SceneComponent : Actor->GetSceneComponents())
	{
		if (!SceneComponent->IsRegistered())
		{
			SceneComponent->RegisterComponentWithWorld(this);
		}
	}

	return Actor;
}

void SWorld::DestroyActor(AActor* InActor)
{
	if (auto It = _Actors.find(InActor); It != _Actors.end())
	{
		for (auto& SceneComponent : InActor->GetSceneComponents())
		{
			if (!SceneComponent->IsRegistered())
			{
				SceneComponent->UnregisterComponent();
			}
		}

		for (auto& OwnedComponent : InActor->GetOwnedComponents())
		{
			if (!OwnedComponent->IsRegistered())
			{
				OwnedComponent->UnregisterComponent();
			}
		}

		UnregisterTickFunction(&InActor->PrimaryActorTick);

		_Actors.erase(It);
		
		if (ensure(InActor->GetOuter() == this))
		{
			DestroySubobject(InActor);
		}
	}
	else
	{
		SE_LOG(LogWorld, Error, L"Could not find actor that desired to destroy.");
		return;
	}
}

SLevel* SWorld::LoadLevel(SubclassOf<SLevel> levelToLoad)
{
	if (!levelToLoad.IsValid())
	{
		SE_LOG(LogWorld, Error, L"The parameter that specified class of desired to load level is nullptr. Abort.");
		return nullptr;
	}

	// Clear spawned actors.
	for (auto& actor : _Actors)
	{
		DestroySubobject(actor);
	}
	_Actors.clear();

	SLevel* levelInstance = levelToLoad.Instantiate(this);
	if (!levelInstance->LoadLevel(this))
	{
		SE_LOG(LogWorld, Fatal, L"Could not load level.");
		DestroySubobject(levelInstance);
		return nullptr;
	}

	// Begin play for all actors.
	_level = levelInstance;
	for (auto& actor : _Actors)
	{
		actor->BeginPlay();
	}

	return levelInstance;
}

const std::set<AActor*>& SWorld::GetAllActors()
{
	return _Actors;
}

std::set<AActor*> SWorld::GetAllActorsOfClass(SubclassOf<AActor> InClass)
{
	std::set<AActor*> ActorsOfClass;
	for (auto& Actor : _Actors)
	{
		if (Actor->GetType()->IsDerivedFrom(InClass.GetType()))
		{
			ActorsOfClass.emplace(Actor);
		}
	}
	return ActorsOfClass;
}

void SWorld::RegisterTickFunction(STickFunction* function)
{
	if (function->bCanEverTick)
	{
		_TickFunctions.Add(function);
	}
}

void SWorld::RegisterComponent(SActorComponent* InComponent)
{
	if (auto* IsPrimitive = Cast<SPrimitiveComponent>(InComponent); IsPrimitive != nullptr)
	{
		int64 Id = -1;

		PrimitiveSceneProxy* SceneProxy = IsPrimitive->CreateSceneProxy();
		if (IsPrimitive->SceneProxy != nullptr)
		{
			Id = IsPrimitive->SceneProxy->PrimitiveId;
			delete IsPrimitive->SceneProxy;
			IsPrimitive->SceneProxy = nullptr;
		}

		if (SceneProxy != nullptr)
		{
			SceneProxy->PrimitiveId = Id;
			if (Id != -1)
			{
				_SceneProxiesToUpdate.emplace_back(SceneProxy);
			}
			else
			{
				_SceneProxiesToRegister.emplace_back(SceneProxy);
			}
		}

		IsPrimitive->SceneProxy = SceneProxy;
	}
}

void SWorld::UnregisterTickFunction(STickFunction* function)
{
	_TickFunctions.Remove(function);
}

void SWorld::UnregisterComponent(SActorComponent* InComponent)
{
	if (auto* IsPrimitive = Cast<SPrimitiveComponent>(InComponent); IsPrimitive != nullptr)
	{
		PrimitiveSceneProxy* SceneProxy = IsPrimitive->SceneProxy;
		if (SceneProxy != nullptr)
		{
			_SceneProxiesToUnregister.emplace_back(SceneProxy);
			SceneProxy = nullptr;
		}

		IsPrimitive->SceneProxy = SceneProxy;
	}
}

void SWorld::LevelTick(std::chrono::duration<float> elapsedTime)
{
	_TickFunctions.ReadyForExecuteTick();

	float Time = elapsedTime.count();

	_TickFunctions.PrePhysics.ExecuteTick(Time);
	_TickFunctions.DuringPhysics.ExecuteTick(Time);
	_TickFunctions.PostPhysics.ExecuteTick(Time);
	_PlayerController->UpdateCameraManager(Time);
	_TickFunctions.PostUpdateWork.ExecuteTick(Time);
}

void SWorld::GetPendingSceneProxies(std::vector<PrimitiveSceneProxy*>& OutToUpdate, std::vector<PrimitiveSceneProxy*>& OutToRegister, std::vector<PrimitiveSceneProxy*>& OutToUnregister)
{
	OutToUpdate = std::move(_SceneProxiesToUpdate);
	OutToRegister = std::move(_SceneProxiesToRegister);
	OutToUnregister = std::move(_SceneProxiesToUnregister);
}