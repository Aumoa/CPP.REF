// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "DirectXDeviceChild.h"

class SDirectXRenderPass;

class DIRECTX_API SDirectXCommandList : public SDirectXDeviceChild, implements IRHICommandBuffer
{
	GENERATED_BODY(SDirectXCommandList)

public:
	ComPtr<ID3D12CommandAllocator> pAllocator;
	ComPtr<ID3D12GraphicsCommandList4> pCommandList;

private:
	SPROPERTY(RunningRenderPass)
	SDirectXRenderPass* RunningRenderPass = nullptr;

public:
	SDirectXCommandList(SDirectXDevice* Owner, ComPtr<ID3D12CommandAllocator> pAllocator, ComPtr<ID3D12GraphicsCommandList4> pCommandList);

	using Super::Dispose;

	virtual void BeginRecord() override;
	virtual void EndRecord() override;
	virtual void ResourceBarrier(std::span<const RHIResourceBarrier> InBarriers) override;
	virtual void ClearRenderTargetView(IRHIRenderTargetView* RTV, int32 IndexOf, const Color& InColor) override;
	virtual void BeginRenderPass(IRHIRenderPass* RenderPass, const RectN& ScissorRect, std::span<Color const> InColor) override;
	virtual void EndRenderPass() override;

protected:
	virtual void Dispose(bool bDisposing) override;
};