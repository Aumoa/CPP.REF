// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Rendering/TextRenderAssetProxy.h"
#include "RHI/RHITextFormat.h"
#include "RHI/RHITextLayout.h"

NTextRenderAssetProxy::NTextRenderAssetProxy(std::shared_ptr<NRHITextLayout> InLayout)
	: Layout(std::move(InLayout))
{
}

NTextRenderAssetProxy::~NTextRenderAssetProxy() noexcept
{
}

bool NTextRenderAssetProxy::TryResolve()
{
	return true;
}

std::shared_ptr<NRHITexture2D> NTextRenderAssetProxy::GetTexture2D() const
{
	if (Layout)
	{
		return Layout->GetTexture2D();
	}
	return nullptr;
}

std::shared_ptr<NRHIShaderResourceView> NTextRenderAssetProxy::GetSRV() const
{
	if (Layout)
	{
		return Layout->GetSRV();
	}
	return nullptr;
}

Rect NTextRenderAssetProxy::GetTextureCoordinate() const
{
	if (Layout)
	{
		return Layout->GetTextureCoordinate();
	}
	return Rect(0.0f, 0.0f, 1.0f, 1.0f);
}

int32 NTextRenderAssetProxy::GetRenderStates() const
{
	return RS_HasTexture | RS_AlphaOnly;
}