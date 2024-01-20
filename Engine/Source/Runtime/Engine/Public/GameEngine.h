// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameObject.h"

class GameAssembly;

class ENGINE_API GameEngine : public GameObject
{
	std::shared_ptr<DynamicLibrary> AssemblyLib;
	std::shared_ptr<GameAssembly> Assembly;

public:
	GameEngine();
	virtual ~GameEngine() noexcept;

	virtual void Initialize();
	virtual void Deinitialize();

protected:
	void StartupGameAssembly();
};