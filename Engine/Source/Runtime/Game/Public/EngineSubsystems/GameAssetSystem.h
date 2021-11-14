// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameEngineSubsystem.h"

class SStaticMesh;
class STexture2D;
class SFont;
class SAssetsLoader;

class GAME_API SGameAssetSystem : public SGameEngineSubsystem
{
	GENERATED_BODY(SGameAssetSystem)

private:
	std::set<std::filesystem::path> AssetsToImport;
	std::map<std::filesystem::path, SObject*> Assets;
	SAssetsLoader* Assimp = nullptr;

public:
	SGameAssetSystem();
	virtual ~SGameAssetSystem() override;

	virtual void Init() override;

	SObject* LoadObject(const std::filesystem::path& assetPath);

private:
	void SearchDirectory(const std::filesystem::path& SearchDirectory);
	void ConvertNativeAssets();
};