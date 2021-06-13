// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:World;

import std.core;
import SC.Runtime.Core;
import :GameConcepts;
import :AActor;
import :SubclassOf;
import :LogGame;
import :TickFunction;

using enum ELogVerbosity;
using namespace std;
using namespace std::chrono;

export class Level;

/// <summary>
/// Represents game world that contains spawned actor, physically state and environment.
/// </summary>
export class World : virtual public Object
{
public:
	using Super = Object;

private:
	set<AActor*> _actors;
	Level* _level = nullptr;
	set<TickFunction*> _tickInstances;

public:
	/// <summary>
	/// Initialize new <see cref="World"/> instance.
	/// </summary>
	World();
	~World();

	/// <summary>
	/// Spawn actor to world.
	/// </summary>
	/// <typeparam name="T"> The actor class. </typeparam>
	/// <returns> Spawned actor. </returns>
	template<derived_from<AActor> T>
	T* SpawnActor()
	{
		T* spawnedActor = CreateSubobject<T>();
		if (!InternalSpawnActor(spawnedActor))
		{
			DestroySubobject(spawnedActor);
			return nullptr;
		}
		return spawnedActor;
	}

	/// <summary>
	/// Spawn actor to world.
	/// </summary>
	/// <param name="actorClass"> The actor class. </typeparam>
	/// <returns> Spawned actor. </returns>
	AActor* SpawnActor(SubclassOf<AActor> actorClass)
	{
		if (!actorClass.IsValid())
		{
			LogSystem::Log(LogWorld, Error, L"Actor class does not specified. Abort.");
			return nullptr;
		}

		AActor* spawnedActor = actorClass.Instantiate(this);
		if (!InternalSpawnActor(spawnedActor))
		{
			DestroySubobject(spawnedActor);
			return nullptr;
		}
		return spawnedActor;
	}

	/// <summary>
	/// Spawn actor to world.
	/// </summary>
	/// <param name="actorClass"> The actor class. </typeparam>
	/// <returns> Spawned actor. </returns>
	template<derived_from<AActor> T>
	T* SpawnActor(SubclassOf<T> actorClass)
	{
		return dynamic_cast<T*>(SpawnActor(SubclassOf<AActor>(actorClass)));
	}

	/// <summary>
	/// Load level.
	/// </summary>
	bool LoadLevel(SubclassOf<Level> levelToLoad);

	void RegisterTickFunction(TickFunction* function);
	virtual void LevelTick(duration<float> elapsedTime);

private:
	bool InternalSpawnActor(AActor* instance);
};