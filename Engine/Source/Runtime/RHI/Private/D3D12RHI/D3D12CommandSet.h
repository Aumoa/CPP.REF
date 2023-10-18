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
	ComPtr<ID3D12GraphicsCommandList4> CommandList;

public:
	ND3D12CommandSet();

	virtual void BeginFrame() override;
	virtual void EndFrame() override;

	virtual void BeginRender(const NRHIViewport& InViewport, bool bClear) override;
	virtual void EndRender(const NRHIViewport& InViewport) override;
	virtual void SetDescriptorHeaps(std::span<NRHIDescriptorHeap* const> Heaps) override;

	virtual void SetSlateShader(const NRHISlateShader& InShader) override;
	virtual void SetScreenResolutionInfo(const Vector2& InConstant) override;
	virtual void SetPaintGeometry(int64 VirtualAddress) override;
	virtual void SetRenderParams(int64 VirtualAddress) override;
	virtual void SetSlateInputTexture(int64 VirtualHandle) override;
	virtual void DrawSlateInstance() override;

	virtual void SetGameShader(const NRHIGameShader& InShader) override;

	ID3D12GraphicsCommandList* GetCommandList() const { return CommandList.Get(); }
};

#endif