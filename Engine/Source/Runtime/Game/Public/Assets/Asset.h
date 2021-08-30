// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include <atomic>

class GAME_API Asset : virtual public Object
{
	CLASS_BODY(Asset)

private:
	std::filesystem::path _assetPath;
	std::atomic<size_t> _lockCount;

public:
	Asset(const std::filesystem::path& path);
	~Asset() override;

	std::filesystem::path GetAssetPath() const;
	
	size_t AddRef();
	size_t ReleaseRef();
};