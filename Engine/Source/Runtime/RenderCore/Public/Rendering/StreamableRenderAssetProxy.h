// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Numerics/VectorInterface/Rect.h"

class NRHITexture2D;
class NRHIShaderResourceView;

class RENDERCORE_API NStreamableRenderAssetProxy : public std::enable_shared_from_this<NStreamableRenderAssetProxy>
{
	NStreamableRenderAssetProxy(const NStreamableRenderAssetProxy&) = delete;
	NStreamableRenderAssetProxy(NStreamableRenderAssetProxy&&) = delete;

protected:
	NStreamableRenderAssetProxy() noexcept = default;

public:
	static constexpr int32 RS_HasTexture = 0x1;
	static constexpr int32 RS_AlphaOnly = 0x2;

public:
	virtual ~NStreamableRenderAssetProxy() noexcept = default;

	virtual bool TryResolve() = 0;
	virtual std::shared_ptr<NRHITexture2D> GetTexture2D() const = 0;
	virtual std::shared_ptr<NRHIShaderResourceView> GetSRV() const = 0;
	virtual Rect GetTextureCoordinate() const = 0;
	virtual int32 GetRenderStates() const = 0;
};