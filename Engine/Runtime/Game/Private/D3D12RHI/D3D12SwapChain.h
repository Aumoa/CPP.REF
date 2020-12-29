// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "RHI/IRHISwapChain.h"

#include "D3D12Minimal.h"

class D3D12Resource;

class D3D12SwapChain : virtual public Object, virtual public IRHISwapChain
{
	using Super = Object;

private:
	ComPtr<IDXGISwapChain4> swapChain;
	size_t bufferCount;
	std::vector<TRefPtr<D3D12Resource>> backBuffers;

public:
	D3D12SwapChain(IDXGISwapChain4* swapChain);
	~D3D12SwapChain() override;

	virtual void Present();
	virtual void ResizeBuffers(int32 width, int32 height);
	virtual TRefPtr<IRHIResource> GetBuffer(size_t index) const;

	virtual size_t CurrentBackBufferIndex_get() const;
	virtual size_t NumBuffers_get() const;

private:
	void ReleaseBuffers();
	void AllocBuffers();
};