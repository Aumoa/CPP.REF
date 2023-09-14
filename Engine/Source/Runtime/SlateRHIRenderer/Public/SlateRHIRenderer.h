// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Rendering/SlateRenderer.h"

class NRHICommandSet;

class SLATERHIRENDERER_API NSlateRHIRenderer : public NSlateRenderer
{
	std::shared_ptr<NRHICommandSet> CommandSet;

public:
	NSlateRHIRenderer();
	virtual ~NSlateRHIRenderer() noexcept override;

	virtual void BeginFrame() override;
	virtual void EndFrame() override;
	virtual void FlushCommands() override;

	virtual void CreateViewport(SWindow& InWindow) override;

	virtual void BeginRender(const NRHIViewport& InViewport) override;
	virtual void EndRender(const NRHIViewport& InViewport) override;
	virtual void RenderElement(const NSlateRenderProxy& InElement) override;
};