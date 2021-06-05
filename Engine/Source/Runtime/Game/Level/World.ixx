// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:World;

import std.core;
import SC.Runtime.Core;
import :GameConcepts;
import :AActor;

using namespace std;

/// <summary>
/// Represents game world that contains spawned actor, physically state and environment.
/// </summary>
export class World : virtual public Object
{
public:
	using Super = Object;

private:
	set<AActor*> actors;

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
	/// <param name="name"> Actor name. </param>
	/// <returns> Spawned actor. </returns>
	template<IsActorClass T>
	T* SpawnActor(wstring_view name)
	{
		T* spawnedActor = CreateSubobject<T>(name);
		if (!InternalSpawnActor(spawnedActor))
		{
			DestroySubobject(spawnedActor);
			return nullptr;
		}
		return spawnedActor;
	}

private:
	bool InternalSpawnActor(AActor* instance);
};