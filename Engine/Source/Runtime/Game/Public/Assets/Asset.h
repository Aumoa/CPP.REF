// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class GAME_API Asset : virtual public Object
{
public:
	using Super = Object;

private:
	std::filesystem::path _assetPath;

public:
	Asset(const std::filesystem::path& path);
	~Asset() override;

	std::filesystem::path GetAssetPath() const;
};