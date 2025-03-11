// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Rendering/StreamableRenderAssetProxy.h"

class RENDERCORE_API NTexture2DTaskRenderAssetProxy : public NStreamableRenderAssetProxy
{
private:
	Task<std::shared_ptr<NRHITexture2D>> TextureTask;
	std::shared_ptr<NRHITexture2D> Texture;
	std::shared_ptr<NRHIShaderResourceView> SRV;

public:
	NTexture2DTaskRenderAssetProxy(Task<std::shared_ptr<NRHITexture2D>> InTextureTask);
	virtual ~NTexture2DTaskRenderAssetProxy() noexcept override;

	virtual bool TryResolve() override;
	virtual std::shared_ptr<NRHITexture2D> GetTexture2D() const override;
	virtual std::shared_ptr<NRHIShaderResourceView> GetSRV() const override;
	virtual Rect GetTextureCoordinate() const override;
	virtual int32 GetRenderStates() const override;
};