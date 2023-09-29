// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Rendering/SlateRenderProxy.h"

class SLATECORE_API NTexture2DTaskRenderProxy : public NSlateRenderProxy
{
private:
	Task<std::shared_ptr<NRHITexture2D>> TextureTask;
	std::shared_ptr<NRHITexture2D> Texture;
	std::shared_ptr<NRHIShaderResourceView> SRV;

public:
	NTexture2DTaskRenderProxy(Task<std::shared_ptr<NRHITexture2D>> InTextureTask);
	virtual ~NTexture2DTaskRenderProxy() noexcept override;

	virtual bool TryResolve() override;
	virtual std::shared_ptr<NRHITexture2D> GetTexture2D() const override;
	virtual std::shared_ptr<NRHIShaderResourceView> GetSRV() const override;
	virtual Rect GetTextureCoordinate() const override;
	virtual int32 GetRenderStates() const override;
};