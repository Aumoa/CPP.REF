// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameEngineSubsystem.h"
#include <filesystem>

class SAssetsLoader;

class GAME_API SGameAssetSystem : public SGameEngineSubsystem
{
	GENERATED_BODY(SGameAssetSystem)

private:
	std::set<std::filesystem::path> AssetsToImport;
	std::map<std::filesystem::path, WeakPtr<SObject>> Assets;
	SPROPERTY(Assimp)
	SAssetsLoader* Assimp = nullptr;

public:
	SGameAssetSystem();
	virtual ~SGameAssetSystem() override;

	virtual void PostInit() override;

	SObject* LoadObject(const std::filesystem::path& AssetPath);

private:
	void SearchDirectory(const std::filesystem::path& SearchDirectory);
	void ConvertNativeAssets();
};