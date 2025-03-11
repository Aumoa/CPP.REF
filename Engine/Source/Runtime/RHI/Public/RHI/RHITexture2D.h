// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIDeviceChild.h"
#include "Numerics/VectorInterface/Vector.h"

class RHI_API NRHITexture2D : public NRHIDeviceChild
{
protected:
	NRHITexture2D() = default;

public:
	virtual ~NRHITexture2D() noexcept override = default;

	virtual Vector2N GetTextureSize() const = 0;
	virtual bool IsRenderTarget() const = 0;
};