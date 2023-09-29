// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Rendering/NullTextureRenderProxy.h"
#include "RHI/RHIShaderResourceView.h"
#include "RHI/RHIGlobal.h"
#include "RHI/RHIGraphics.h"

NNullTextureRenderProxy::NNullTextureRenderProxy()
{
	SRV = NRHIGlobal::GetDynamicRHI().CreateShaderResourceView(1);
	SRV->CreateView(0, nullptr);
}

NNullTextureRenderProxy::~NNullTextureRenderProxy() noexcept
{
}

bool NNullTextureRenderProxy::TryResolve()
{
	return true;
}

std::shared_ptr<NRHITexture2D> NNullTextureRenderProxy::GetTexture2D() const
{
	return nullptr;
}

std::shared_ptr<NRHIShaderResourceView> NNullTextureRenderProxy::GetSRV() const
{
	return SRV;
}

Rect NNullTextureRenderProxy::GetTextureCoordinate() const
{
	return Rect(0.0f, 0.0f, 1.0f, 1.0f);
}

int32 NNullTextureRenderProxy::GetRenderStates() const
{
	return 0;
}