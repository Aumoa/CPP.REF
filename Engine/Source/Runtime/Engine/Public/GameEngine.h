// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameObject.h"
#include "GameInstance.h"

class GameAssembly;

class ENGINE_API GameEngine : public GameObject
{
	std::shared_ptr<DynamicLibrary> AssemblyLib;
	std::shared_ptr<GameAssembly> Assembly;

	std::shared_ptr<GameInstance> CurrentGame;

public:
	GameEngine();
	virtual ~GameEngine() noexcept;

	virtual void Initialize();
	virtual void Deinitialize();

	void StartGame();
	void EndGame();

protected:
	void StartupGameAssembly();
};