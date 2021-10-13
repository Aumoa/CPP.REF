// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameObject.h"
#include <set>
#include <ranges>
#include "SubclassOf.h"
#include "LogGame.h"
#include "GameFramework/Actor.h"

class SLevel;
class SGameEngine;
class APlayerController;
class APlayerCameraManager;
class STickFunction;
class PrimitiveSceneProxy;

/// <summary>
/// Represents game world that contains spawned actor, physically state and environment.
/// </summary>
class GAME_API SWorld : public SGameObject
{
	GENERATED_BODY(SWorld)

private:
	template<ETickingGroup _Group>
	struct TickGroup
	{
		static constexpr ETickingGroup Group = _Group;
		std::set<STickFunction*> Functions;

		bool Add(STickFunction* function);
		bool Remove(STickFunction* function);

		void ReadyForExecuteTick();
		void ExecuteTick(float elapsedTime);
	};

	struct TickFunctions
	{
		TickGroup<ETickingGroup::PrePhysics> PrePhysics;
		TickGroup<ETickingGroup::DuringPhysics> DuringPhysics;
		TickGroup<ETickingGroup::PostPhysics> PostPhysics;
		TickGroup<ETickingGroup::PostUpdateWork> PostUpdateWork;

		bool Add(STickFunction* function);
		bool Remove(STickFunction* function);

		void ReadyForExecuteTick();
	};

	SLevel* _level = nullptr;

	std::set<AActor*> _Actors;
	TickFunctions _TickFunctions;

	std::vector<PrimitiveSceneProxy*> _SceneProxiesToUpdate;
	std::vector<PrimitiveSceneProxy*> _SceneProxiesToRegister;
	std::vector<PrimitiveSceneProxy*> _SceneProxiesToUnregister;

	APlayerController* _PlayerController = nullptr;
	APlayerCameraManager* _PlayerCamera = nullptr;

public:
	/// <summary>
	/// Initialize new <see cref="SWorld"/> instance.
	/// </summary>
	SWorld();

	virtual SWorld* GetWorld() override;

	/// <summary>
	/// Spawn actor to world.
	/// </summary>
	/// <typeparam name="T"> The actor class. </typeparam>
	/// <returns> Spawned actor. </returns>
	template<std::derived_from<AActor> T>
	T* SpawnActor() { return SpawnActor<T>(T::StaticClass()); }

	/// <summary>
	/// Spawn actor to world.
	/// </summary>
	/// <typeparam name="T"> The actor class. </typeparam>
	/// <returns> Spawned actor. </returns>
	template<std::derived_from<AActor> T>
	T* SpawnActor(SubclassOf<T> InActorClass) { return static_cast<T*>(SpawnActor((SubclassOf<AActor>)InActorClass)); }

	/// <summary>
	/// Spawn actor to world.
	/// </summary>
	/// <param name="InActorClass"> The actor class. </typeparam>
	/// <returns> Spawned actor. </returns>
	AActor* SpawnActor(SubclassOf<AActor> InActorClass);
	void DestroyActor(AActor* InActor);

	/// <summary>
	/// Load level.
	/// </summary>
	SLevel* LoadLevel(SubclassOf<SLevel> levelToLoad);

	const std::set<AActor*>& GetAllActors();
	std::set<AActor*> GetAllActorsOfClass(SubclassOf<AActor> InClass);

	template<std::derived_from<AActor> T>
	std::set<T*> GetAllActorsOfClass()
	{
		std::set<T*> ActorsOfClass;
		for (auto& Actor : _Actors)
		{
			if (auto* Ptr = dynamic_cast<T*>(Actor); Ptr)
			{
				ActorsOfClass.emplace(Ptr);
			}
		}
		return ActorsOfClass;
	}

	void RegisterTickFunction(STickFunction* function);
	void RegisterComponent(SActorComponent* InComponent);
	void UnregisterTickFunction(STickFunction* function);
	void UnregisterComponent(SActorComponent* InComponent);

	virtual void LevelTick(std::chrono::duration<float> elapsedTime);
	APlayerCameraManager* GetPlayerCamera() const { return _PlayerCamera; }
	SLevel* GetLevel() const { return _level; }

	void GetPendingSceneProxies(std::vector<PrimitiveSceneProxy*>& OutToUpdate, std::vector<PrimitiveSceneProxy*>& OutToRegister, std::vector<PrimitiveSceneProxy*>& OutToUnregister);
};