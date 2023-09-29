// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIDeviceChild.h"
#include "Numerics/VectorInterface/Vector.h"

class RHI_API NRHIViewport : public NRHIDeviceChild
{
protected:
	NRHIViewport() = default;

public:
	virtual void Present() = 0;
	virtual Vector2N GetViewportSize() const = 0;
	virtual void Resize(const Vector2N& InSize) = 0;
};