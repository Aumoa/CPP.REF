// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIDeviceChild.h"
#include "Numerics/VectorInterface/Vector.h"

class NRHITexture2D;

class RHI_API NRHIViewport : public NRHIDeviceChild
{
protected:
	NRHIViewport() = default;

public:
	virtual Vector2N GetViewportSize() const = 0;
	virtual void Resize(const Vector2N& InSize) = 0;
	virtual std::shared_ptr<NRHITexture2D> GetTexture() const = 0;
};