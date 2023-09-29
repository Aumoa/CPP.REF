// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Rendering/SlateRenderProxy.h"

class SLATECORE_API NNullTextureRenderProxy : public NSlateRenderProxy
{
private:
	std::shared_ptr<NRHIShaderResourceView> SRV;

public:
	NNullTextureRenderProxy();
	virtual ~NNullTextureRenderProxy() noexcept override;

	virtual bool TryResolve() override;
	virtual std::shared_ptr<NRHITexture2D> GetTexture2D() const override;
	virtual std::shared_ptr<NRHIShaderResourceView> GetSRV() const override;
	virtual Rect GetTextureCoordinate() const override;
	virtual int32 GetRenderStates() const override;
};