// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Assets/SoftObjectPtr.h"
#include "Assets/Asset.h"
#include "GameEngine.h"
#include "LogGame.h"
#include "GameObject.h"

SAsset* SoftObjectPtrBase::InternalLoadObject(const std::filesystem::path& importPath)
{
	if (GEngine == nullptr)
	{
		SE_LOG(LogAssets, Fatal, L"Engine instance does not initialized.");
		return nullptr;
	}

	return SGameObject::LoadObject<SAsset>(importPath);
}

void SoftObjectPtrBase::InternalUnloadObject(const std::filesystem::path& importPath)
{
	if (GEngine)
	{
		SE_LOG(LogAssets, Fatal, L"Engine instance does not initialized.");
		return;
	}
}