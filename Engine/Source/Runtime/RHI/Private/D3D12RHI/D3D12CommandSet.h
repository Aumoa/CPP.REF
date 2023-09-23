// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHICommandSet.h"

#if PLATFORM_WINDOWS

#include "D3D12RHI/D3D12Common.h"

class ND3D12CommandSet : public NRHICommandSet
{
private:
	ComPtr<ID3D12CommandAllocator> Allocator;
	ComPtr<ID3D12GraphicsCommandList> CommandList;

public:
	ND3D12CommandSet();

	virtual void BeginFrame(const NRHIGraphicsPipelineState* pInitialPipeline) override;
	virtual void EndFrame() override;

	virtual void BeginRender(const NRHIViewport& InViewport, bool bClear) override;
	virtual void EndRender(const NRHIViewport& InViewport) override;

	virtual void SetGraphicsRootSignature(NRHIRootSignature& InRS) override;
	virtual void SetGraphicsRoot32BitConstants(int32 RootParameterIndex, int32 Num32BitValuesToSet, const void* pSrcData, int32 DestOffsetIn32BitValues) override;
	virtual void SetGraphicsRootConstantBufferView(int32 RootParameterIndex, int64 BufferLocation) override;

	virtual void DrawInstanced(bool bStrip, int32 InVertexCount, int32 InInstanceCount, int32 InVertexStart, int32 InInstanceStart) override;

	ID3D12GraphicsCommandList* GetCommandList() const { return CommandList.Get(); }
};

#endif