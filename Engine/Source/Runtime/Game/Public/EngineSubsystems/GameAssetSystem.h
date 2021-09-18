// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameEngineSubsystem.h"

class STexture2D;
class SFont;

class GAME_API SGameAssetSystem : public SGameEngineSubsystem
{
	GENERATED_BODY(SGameAssetSystem)

private:
	std::map<std::filesystem::path, SObject*> _assets;

public:
	SGameAssetSystem();
	virtual ~SGameAssetSystem() override;

	virtual void Init() override;

	SObject* LoadObject(const std::filesystem::path& assetPath);

private:
	STexture2D* LoadTexture2D(const std::filesystem::path& assetPath);
	SFont* LoadFont(const std::filesystem::path& assetPath);
};