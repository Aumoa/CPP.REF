// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Rendering/SlateRenderProxy.h"

class NRHITextLayout;

class SLATECORE_API NTextRenderProxy : public NSlateRenderProxy
{
	std::shared_ptr<NRHITextLayout> Layout;

public:
	NTextRenderProxy(std::shared_ptr<NRHITextLayout> InLayout);
	virtual ~NTextRenderProxy() noexcept override;

	virtual bool TryResolve() override;
	virtual std::shared_ptr<NRHITexture2D> GetTexture2D() const override;
	virtual std::shared_ptr<NRHIShaderResourceView> GetSRV() const override;
	virtual Rect GetTextureCoordinate() const override;
};