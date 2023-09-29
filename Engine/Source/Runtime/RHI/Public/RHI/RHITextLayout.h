// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIDeviceChild.h"
#include "Numerics/VectorInterface/Vector.h"
#include "Numerics/VectorInterface/Rect.h"

class NRHITextFormat;
class NRHITexture2D;
class NRHIShaderResourceView;

class RHI_API NRHITextLayout : public NRHIDeviceChild
{
protected:
	NRHITextLayout() = default;

public:
	virtual ~NRHITextLayout() noexcept override = default;

	virtual String GetText() const = 0;
	virtual Vector2 GetDrawingSize() const = 0;
	virtual Rect GetTextureCoordinate() const = 0;
	virtual std::shared_ptr<NRHITextFormat> GetTextFormat() const = 0;

	virtual std::shared_ptr<NRHITexture2D> GetTexture2D() const = 0;
	virtual std::shared_ptr<NRHIShaderResourceView> GetSRV() const = 0;
};