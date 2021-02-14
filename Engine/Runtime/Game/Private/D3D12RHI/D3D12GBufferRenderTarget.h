// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "D3D12RenderTarget.h"

class D3D12DeviceBundle;
class D3D12IndependentShaderResourceView;
class D3D12Resource;

class D3D12GBufferRenderTarget : public D3D12RenderTarget
{
public:
	using Super = D3D12RenderTarget;
	using This = D3D12GBufferRenderTarget;

private:
	D3D12DeviceBundle* bundle;
	ID3D12Device* device;

	int32 width;
	int32 height;

	ComPtr<ID3D12DescriptorHeap> rtvHeap;
	ComPtr<ID3D12DescriptorHeap> dsvHeap;
	TRefPtr<D3D12IndependentShaderResourceView> srvHeap;
	uint32 rtvIncrement;

	TRefPtr<D3D12Resource> colorBuffer;
	TRefPtr<D3D12Resource> normalBuffer;
	TRefPtr<D3D12Resource> depthStencilBuffer;

public:
	D3D12GBufferRenderTarget();
	~D3D12GBufferRenderTarget() override;

	virtual void ResizeBuffers(int32 width, int32 height);
	virtual size_t GetRenderTargetCount() const;
	virtual IRHIResource* GetRenderTarget(size_t index) const;
	virtual IRHIShaderResourceView* GetShaderResourceView() const;

	void BeginRender(ID3D12GraphicsCommandList* inCommandList) override;
	void EndRender(ID3D12GraphicsCommandList* inCommandList) override;

private:
	TRefPtr<D3D12Resource> CreateTexture2D(DXGI_FORMAT format, D3D12_RESOURCE_STATES initialState, int32 width, int32 height);
};