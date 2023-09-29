// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Object.h"
#include "Rendering/StreamableRenderAssetProxy.h"
#include "StreamableRenderAsset.generated.h"

ACLASS()
class ENGINE_API AStreamableRenderAsset : public AObject
{
	GENERATED_BODY(AStreamableRenderAsset)

public:
	virtual bool IsStreamedIn() const = 0;

	virtual void StreamIn() = 0;
	virtual void StreamOut() = 0;
	virtual std::shared_ptr<NStreamableRenderAssetProxy> GetRenderProxy() const = 0;
};