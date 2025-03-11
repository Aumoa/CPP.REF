// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIDeviceChild.h"

class NRHITexture2D;

class RHI_API NRHIShaderResourceView : public NRHIDeviceChild
{
protected:
	NRHIShaderResourceView() noexcept = default;

public:
	virtual ~NRHIShaderResourceView() noexcept override = default;

	virtual size_t GetNumViews() const = 0;
	virtual void CreateView(size_t Index, NRHITexture2D* InResource) = 0;
};