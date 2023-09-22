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

	virtual void BeginFrame(const NRHIShader* pInitShader) override;
	virtual void EndFrame() override;

	virtual void BeginRender(const NRHIViewport& InViewport, bool bClear) override;
	virtual void EndRender(const NRHIViewport& InViewport) override;

	ID3D12GraphicsCommandList* GetCommandList() const { return CommandList.Get(); }
};

#endif