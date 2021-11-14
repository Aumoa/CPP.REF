// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

interface IRHIDevice;

class ASSETSIO_API SAssetsLoader : implements SObject
{
	GENERATED_BODY(SAssetsLoader)

private:
	static constexpr uint32 ImporterVersion = 1;
	IRHIDevice* Device = nullptr;

public:
	SAssetsLoader(IRHIDevice* InDevice);

	SObject* ImportFromFile(const std::filesystem::path& AssetPath);
	bool ConvertAssets(const std::filesystem::path& NativeAssetPath, const std::filesystem::path& ConvertPath);

	IRHIDevice* GetDevice();
};