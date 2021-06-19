// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SubclassOf.h"

class World;
class AGameMode;

/// <summary>
/// Represents actor placement unit.
/// </summary>
class Level : virtual public Object
{
public:
	using Super = Object;

public:
	SubclassOf<AGameMode> GameModeClass;

private:
	AGameMode* _gameMode = nullptr;

public:
	/// <summary>
	/// Initialize new <see cref="Level"/> instance.
	/// </summary>
	Level();
	~Level();

	/// <summary>
	/// Load level.
	/// </summary>
	/// <param name="world"> The world that level be placed. </param>
	virtual bool LoadLevel(World* world);
};