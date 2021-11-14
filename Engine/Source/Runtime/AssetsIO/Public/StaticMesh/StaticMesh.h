// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "StreamableRenderAsset.h"

class ASSETSIO_API SStaticMesh : public SStreamableRenderAsset
{
	GENERATED_BODY(SStaticMesh)

public:
	SStaticMesh();

	virtual bool StreamIn(SAssetsLoader* Loader, std::span<const uint8> AssetsBin) override;
};