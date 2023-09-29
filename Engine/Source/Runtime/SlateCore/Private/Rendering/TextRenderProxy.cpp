// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Rendering/TextRenderProxy.h"
#include "RHI/RHITextFormat.h"
#include "RHI/RHITextLayout.h"

NTextRenderProxy::NTextRenderProxy(std::shared_ptr<NRHITextLayout> InLayout)
	: Layout(std::move(InLayout))
{
}

NTextRenderProxy::~NTextRenderProxy() noexcept
{
}

bool NTextRenderProxy::TryResolve()
{
	return true;
}

std::shared_ptr<NRHITexture2D> NTextRenderProxy::GetTexture2D() const
{
	if (Layout)
	{
		return Layout->GetTexture2D();
	}
	return nullptr;
}

std::shared_ptr<NRHIShaderResourceView> NTextRenderProxy::GetSRV() const
{
	if (Layout)
	{
		return Layout->GetSRV();
	}
	return nullptr;
}

Rect NTextRenderProxy::GetTextureCoordinate() const
{
	if (Layout)
	{
		return Layout->GetTextureCoordinate();
	}
	return Rect(0.0f, 0.0f, 1.0f, 1.0f);
}

int32 NTextRenderProxy::GetRenderStates() const
{
	return RS_HasTexture | RS_AlphaOnly;
}