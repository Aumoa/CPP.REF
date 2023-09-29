// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RefPtr.h"

class AStreamableRenderAsset;

struct ENGINE_API NStreamableAssetManager : public StaticClass
{
	static RefPtr<AStreamableRenderAsset> LoadObject(String InPath);

private:
	static std::map<String, WeakPtr<AStreamableRenderAsset>> StreamableAssets;
};

inline RefPtr<AStreamableRenderAsset> LoadObject(String InPath)
{
	return NStreamableAssetManager::LoadObject(InPath);
}