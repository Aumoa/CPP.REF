// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Rendering/SlateRenderer.h"

class NRHICommandSet;
class NRHIConstantBuffer;
class NRHIDescriptorHeap;

class SLATERHIRENDERER_API NSlateRHIRenderer : public NSlateRenderer
{
	struct NViewportCommands
	{
		std::shared_ptr<NRHICommandSet> CommandSet;

		std::shared_ptr<NRHIConstantBuffer> ConstantBuffers;
		size_t ConstantBufferUsage = 0;

		std::shared_ptr<NRHIDescriptorHeap> DescriptorHeap;
		size_t DescriptorUsage = 0;

		void Restart()
		{
			ConstantBufferUsage = 0;
			DescriptorUsage = 0;
		}
	};

	std::vector<NViewportCommands> CachedVpCommands;
	size_t VpIndex = 0;

public:
	NSlateRHIRenderer();
	virtual ~NSlateRHIRenderer() noexcept override;

	virtual void BeginFrame() override;
	virtual void EndFrame() override;
	virtual void FlushCommands() override;

	virtual void CreateViewport(SWindow& InWindow) override;

	virtual void BeginRender(const NRHIViewport& InViewport) override;
	virtual void EndRender(const NRHIViewport& InViewport) override;
	virtual void RenderElement(const NSlateRenderElement& InElement) override;

	virtual void Populate(const NSlateWindowElementList& InElementList) override;
};