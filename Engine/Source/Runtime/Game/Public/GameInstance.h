// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameObject.h"
#include "SubclassOf.h"

class Level;

/// <summary>
/// Represents single game instance while application are running.
/// </summary>
class GAME_API GameInstance : public GameObject
{
	GENERATED_BODY(GameInstance)

public:
	SubclassOf<Level> StartupLevel;

public:
	/// <summary>
	/// Initialize new <see cref="GameInstance"/> instance.
	/// </summary>
	GameInstance();
};

#define DEFINE_GAME_MODULE(GameInstanceClass)			\
extern "C"												\
{														\
	__declspec(dllexport)								\
	GameInstance* LoadGameInstance()					\
	{													\
		return new GameInstanceClass();					\
	}													\
}