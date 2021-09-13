// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Assets/SoftObjectPtr.h"
#include "Assets/AssetImporter.h"
#include "GameEngine.h"
#include "LogGame.h"

SAsset* SoftObjectPtrBase::InternalLoadObject(const std::filesystem::path& importPath)
{
	if (GEngine == nullptr)
	{
		SE_LOG(LogAssets, Fatal, L"Engine instance does not initialized.");
		return nullptr;
	}

	//SAssetImporter* importer = GEngine->GetAssetImporter();
	//return importer->LoadObject(importPath);
	return nullptr;
}

void SoftObjectPtrBase::InternalUnloadObject(const std::filesystem::path& importPath)
{
	if (GEngine)
	{
		SE_LOG(LogAssets, Fatal, L"Engine instance does not initialized.");
		return;
	}

	//SAssetImporter* importer = engine->GetAssetImporter();
	//importer->UnloadObject(importPath);
}