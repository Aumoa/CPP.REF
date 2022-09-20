// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "GameEngine.gen.h"

class World;
class SceneView;

SCLASS()
class GAME_API GameEngine : public Engine
{
	GENERATED_BODY()

private:
	SPROPERTY()
	World* GameWorld = nullptr;
	std::unique_ptr<SceneView> GameViewport;

public:
	GameEngine();
	virtual ~GameEngine() noexcept override;

	virtual void Init() override;
	virtual void Start(GameInstance* Instance) override;

protected:
	virtual void DispatchEngineTick(const TimeSpan& InDeltaTime);

private:
	World* SpawnGameWorld();
};