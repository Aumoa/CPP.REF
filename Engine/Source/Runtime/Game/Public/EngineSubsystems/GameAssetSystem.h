// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameEngineSubsystem.h"

class Texture2D;

class GAME_API GameAssetSystem : public GameEngineSubsystem
{
	GENERATED_BODY(GameAssetSystem)

private:
	std::map<std::filesystem::path, Object*> _assets;

public:
	GameAssetSystem();
	virtual ~GameAssetSystem() override;

	virtual void Init() override;

	Object* LoadObject(const std::filesystem::path& assetPath);

private:
	Texture2D* LoadTexture2D(const std::filesystem::path& assetPath);
};