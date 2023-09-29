// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Rendering/StreamableRenderAssetProxy.h"

class NRHITextLayout;

class RENDERCORE_API NTextRenderAssetProxy : public NStreamableRenderAssetProxy
{
	std::shared_ptr<NRHITextLayout> Layout;

public:
	NTextRenderAssetProxy(std::shared_ptr<NRHITextLayout> InLayout);
	virtual ~NTextRenderAssetProxy() noexcept override;

	virtual bool TryResolve() override;
	virtual std::shared_ptr<NRHITexture2D> GetTexture2D() const override;
	virtual std::shared_ptr<NRHIShaderResourceView> GetSRV() const override;
	virtual Rect GetTextureCoordinate() const override;
	virtual int32 GetRenderStates() const override;
};