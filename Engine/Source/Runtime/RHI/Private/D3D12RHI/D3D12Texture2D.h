// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHITexture2D.h"

#if PLATFORM_WINDOWS

#include "D3D12RHI/D3D12Common.h"

class ND3D12Texture2D : public NRHITexture2D
{
	ComPtr<ID3D12Resource> Resource;

public:
	ND3D12Texture2D(ComPtr<ID3D12Resource> InResource);
	virtual ~ND3D12Texture2D() noexcept override;

	ID3D12Resource* GetResource() const noexcept { return Resource.Get(); }
};

#endif