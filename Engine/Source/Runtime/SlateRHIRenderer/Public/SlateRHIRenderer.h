// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Rendering/SlateRenderer.h"

class NRHICommandSet;
class NRHIStructuredBuffer;
class NRHIDescriptorHeap;
class NRHIShaderResourceView;

class SLATERHIRENDERER_API NSlateRHIRenderer : public NSlateRenderer
{
	struct NViewportCommands
	{
		struct NLayered
		{
			int32 Layer = 0;
			int32 NumElements = 0;
			NRHIShaderResourceView* pSRV = nullptr;

			int64 VPaintGeometryStart = 0;
			int64 VRenderParamsStart = 0;
			int64 VSRVHandle = 0;
		};

		std::shared_ptr<NRHICommandSet> CommandSet;
		std::vector<NLayered> Layers;

		std::shared_ptr<NRHIStructuredBuffer> StructuredBuffers;
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
	virtual void DrawLayered(const NSlateWindowElementList& InElementList) override;
};