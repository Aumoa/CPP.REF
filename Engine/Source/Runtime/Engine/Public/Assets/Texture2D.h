// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Assets/StreamableRenderAsset.h"
#include "Texture2D.generated.h"

class NRHITexture2D;

ACLASS()
class ENGINE_API ATexture2D : public AStreamableRenderAsset
{
	GENERATED_BODY()

private:
	String AbsolutePath;
	std::shared_ptr<NStreamableRenderAssetProxy> RenderProxy;

public:
	virtual bool IsStreamedIn() const override;

	virtual void StreamIn() override;
	virtual void StreamOut() override;
	virtual std::shared_ptr<NStreamableRenderAssetProxy> GetRenderProxy() const override;

public:
	static RefPtr<ATexture2D> LoadAndStreamIn(String InAbsolutePath);
};