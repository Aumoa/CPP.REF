// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHITexture2D.h"

#if PLATFORM_WINDOWS

#include "D3D12RHI/D3D12Common.h"

class ND3D12Texture2D : public NRHITexture2D
{
	ComPtr<ID3D12Resource> Resource;
	D3D12_RESOURCE_DESC Desc;

public:
	ND3D12Texture2D(ComPtr<ID3D12Resource> InResource, const D3D12_RESOURCE_DESC& InDesc);
	virtual ~ND3D12Texture2D() noexcept override;

	virtual Vector2N GetTextureSize() const override;
	virtual bool IsRenderTarget() const override;

	ID3D12Resource* GetResource() const noexcept { return Resource.Get(); }
};

#endif