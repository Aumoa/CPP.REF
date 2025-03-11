// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIViewport.h"

#if PLATFORM_WINDOWS

#include "D3D12RHI/D3D12Common.h"

class ND3D12Texture2D;
class ND3D12ShaderResourceView;

class ND3D12Viewport : public NRHIViewport
{
	ID3D12Device* pDevice = nullptr;

	ComPtr<ID3D12Resource> pResource;
	ComPtr<ID3D12DescriptorHeap> pRTVHeap;
	Vector2N VpSize;
	std::shared_ptr<ND3D12Texture2D> Texture;
	std::shared_ptr<ND3D12ShaderResourceView> SRV;

public:
	ND3D12Viewport(ID3D12Device* pInDevice);
	virtual ~ND3D12Viewport() noexcept override;

	virtual Vector2N GetViewportSize() const override;
	virtual void Resize(const Vector2N& InSize) override;
	virtual std::shared_ptr<NRHITexture2D> GetTexture() const override;

	D3D12_CPU_DESCRIPTOR_HANDLE GetRTVHandle() const { return pRTVHeap->GetCPUDescriptorHandleForHeapStart(); }
	ComPtr<ID3D12Resource> GetResource() const { return pResource; }
};

#endif