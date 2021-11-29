// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class SAssetsLoader;

interface IAssets : implements SObject
{
	virtual bool StreamIn(SAssetsLoader* Loader, std::span<const uint8> AssetsBin) = 0;
};