// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class SWindow;
class NRHIViewport;
struct NSlateRenderElement;
struct NSlateWindowElementList;

class SLATECORE_API NSlateRenderer
{
	NSlateRenderer(const NSlateRenderer&) = delete;
	NSlateRenderer(NSlateRenderer&&) = delete;

protected:
	NSlateRenderer();

public:
	virtual ~NSlateRenderer() noexcept;

	virtual void BeginFrame() {}
	virtual void EndFrame() {}
	virtual void FlushCommands() {}

	virtual void CreateViewport(SWindow& InWindow) = 0;

	virtual void BeginRender(const NRHIViewport& InViewport) = 0;
	virtual void EndRender(const NRHIViewport& InViewport) = 0;
	virtual void RenderElement(const NSlateRenderElement& InElement) = 0;

	virtual void Populate(const NSlateWindowElementList& InElementList) = 0;
};