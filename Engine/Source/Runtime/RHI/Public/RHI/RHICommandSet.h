// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIDeviceChild.h"

class NRHIViewport;
class NRHIRootSignature;
class NRHIGraphicsPipelineState;

class RHI_API NRHICommandSet : public NRHIDeviceChild
{
protected:
	NRHICommandSet() = default;

public:
	virtual ~NRHICommandSet() noexcept override = default;

	virtual void BeginFrame(const NRHIGraphicsPipelineState* pInitialPipeline) = 0;
	virtual void EndFrame() = 0;

	virtual void BeginRender(const NRHIViewport& InViewport, bool bClear) = 0;
	virtual void EndRender(const NRHIViewport& InViewport) = 0;

	virtual void SetGraphicsRootSignature(NRHIRootSignature& InRS) = 0;
	virtual void SetGraphicsRoot32BitConstants(int32 RootParameterIndex, int32 Num32BitValuesToSet, const void* pSrcData, int32 DestOffsetIn32BitValues) = 0;
	virtual void SetGraphicsRootConstantBufferView(int32 RootParameterIndex, int64 BufferLocation) = 0;

	virtual void DrawInstanced(bool bStrip, int32 InVertexCount, int32 InInstanceCount, int32 InVertexStart, int32 InInstanceStart) = 0;
};