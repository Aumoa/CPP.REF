// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "D3D12RenderTarget.h"

#include "D3D12Minimal.h"

class D3D12DeviceBundle;
class D3D12IndependentShaderResourceView;
class D3D12IndependentUnorderedAccessView;
class D3D12Resource;

class D3D12HDRRenderTarget : public D3D12RenderTarget
{
public:
	using Super = D3D12RenderTarget;

private:
	D3D12DeviceBundle* bundle;
	ID3D12Device* device;

	int32 width;
	int32 height;

	TRefPtr<D3D12IndependentShaderResourceView> srvHeap;
	TRefPtr<D3D12IndependentUnorderedAccessView> uavHeap;

	TRefPtr<D3D12Resource> colorBuffer;
	
public:
	D3D12HDRRenderTarget(D3D12DeviceBundle* owner);
	~D3D12HDRRenderTarget() override;

	virtual void ResizeBuffers(int32 width, int32 height);
	virtual size_t GetRenderTargetCount() const;
	virtual IRHIResource* GetRenderTarget(size_t index) const;
	virtual IRHIShaderResourceView* GetShaderResourceView() const;
	virtual IRHIUnorderedAccessView* GetUnorderedAccessView() const;
	virtual uint32 Width_get() const;
	virtual uint32 Height_get() const;

	void BeginRender(ID3D12GraphicsCommandList4* inCommandList) override;
	void EndRender(ID3D12GraphicsCommandList4* inCommandList) override;
};