// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Assets/SoftObjectPtr.h"
#include "Assets/AssetImporter.h"
#include "GameEngine.h"
#include "LogGame.h"

Asset* SoftObjectPtrBase::InternalLoadObject(const std::filesystem::path& importPath)
{
	GameEngine* engine = GameEngine::GetEngine();
	if (engine == nullptr)
	{
		SE_LOG(LogAssets, Fatal, L"Engine instance does not initialized.");
		return nullptr;
	}

	//AssetImporter* importer = engine->GetAssetImporter();
	//return importer->LoadObject(importPath);
	return nullptr;
}

void SoftObjectPtrBase::InternalUnloadObject(const std::filesystem::path& importPath)
{
	GameEngine* engine = GameEngine::GetEngine();
	if (engine == nullptr)
	{
		SE_LOG(LogAssets, Fatal, L"Engine instance does not initialized.");
		return;
	}

	//AssetImporter* importer = engine->GetAssetImporter();
	//importer->UnloadObject(importPath);
}