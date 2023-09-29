// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Assets/StreamableAssetManager.h"
#include "Assets/StreamableRenderAsset.h"
#include "Assets/AssetsPath.h"
#include "Assets/Texture2D.h"

std::map<String, WeakPtr<AStreamableRenderAsset>> NStreamableAssetManager::StreamableAssets;

RefPtr<AStreamableRenderAsset> NStreamableAssetManager::LoadObject(String InPath)
{
	auto It = StreamableAssets.find(InPath);
	if (It != StreamableAssets.end())
	{
		RefPtr<AStreamableRenderAsset> Asset = It->second.Lock();
		if (Asset)
		{
			return Asset;
		}
		StreamableAssets.erase(It);
	}

	String FSPath = NAssetsPath::GetFileSystemPath(InPath);
	if (String PNGPath = Path::ChangeExtension(FSPath, TEXT(".png")); File::Exists(PNGPath))
	{
		return ATexture2D::LoadAndStreamIn(PNGPath);
	}

	checkf(false, TEXT("asset is not exists."));
	return nullptr;
}