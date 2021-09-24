// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include <atomic>

class GAME_API SAsset : implements SObject
{
	GENERATED_BODY(SAsset)

private:
	std::filesystem::path _assetPath;
	std::atomic<size_t> _lockCount;

public:
	SAsset(const std::filesystem::path& path);
	~SAsset() override;

	std::filesystem::path GetAssetPath() const;
	
	size_t AddRef();
	size_t ReleaseRef();
};