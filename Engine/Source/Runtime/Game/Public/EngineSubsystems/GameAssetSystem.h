// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameEngineSubsystem.h"

class GAME_API GameAssetSystem : public GameEngineSubsystem
{
	CLASS_BODY(GameAssetSystem)

private:
	std::map<std::filesystem::path, Object*> _assets;

public:
	GameAssetSystem();
	virtual ~GameAssetSystem() override;

	virtual void Init() override;

	Object* LoadObject(const std::filesystem::path& assetPath);
};