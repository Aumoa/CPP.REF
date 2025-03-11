// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Rendering/NullTextureRenderAssetProxy.h"
#include "RHI/RHIShaderResourceView.h"
#include "RHI/RHIGlobal.h"
#include "RHI/RHIGraphics.h"

NNullTextureRenderAssetProxy::NNullTextureRenderAssetProxy()
{
	SRV = NRHIGlobal::GetDynamicRHI().CreateShaderResourceView(1);
	SRV->CreateView(0, nullptr);
}

NNullTextureRenderAssetProxy::~NNullTextureRenderAssetProxy() noexcept
{
}

bool NNullTextureRenderAssetProxy::TryResolve()
{
	return true;
}

std::shared_ptr<NRHITexture2D> NNullTextureRenderAssetProxy::GetTexture2D() const
{
	return nullptr;
}

std::shared_ptr<NRHIShaderResourceView> NNullTextureRenderAssetProxy::GetSRV() const
{
	return SRV;
}

Rect NNullTextureRenderAssetProxy::GetTextureCoordinate() const
{
	return Rect(0.0f, 0.0f, 1.0f, 1.0f);
}

int32 NNullTextureRenderAssetProxy::GetRenderStates() const
{
	return 0;
}