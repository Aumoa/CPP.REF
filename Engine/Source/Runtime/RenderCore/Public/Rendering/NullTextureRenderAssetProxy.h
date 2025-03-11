// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Rendering/StreamableRenderAssetProxy.h"

class RENDERCORE_API NNullTextureRenderAssetProxy : public NStreamableRenderAssetProxy
{
private:
	std::shared_ptr<NRHIShaderResourceView> SRV;

public:
	NNullTextureRenderAssetProxy();
	virtual ~NNullTextureRenderAssetProxy() noexcept override;

	virtual bool TryResolve() override;
	virtual std::shared_ptr<NRHITexture2D> GetTexture2D() const override;
	virtual std::shared_ptr<NRHIShaderResourceView> GetSRV() const override;
	virtual Rect GetTextureCoordinate() const override;
	virtual int32 GetRenderStates() const override;
};