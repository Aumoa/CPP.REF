// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Rendering/Texture2DTaskRenderAssetProxy.h"
#include "RHI/RHITexture2D.h"
#include "RHI/RHIShaderResourceView.h"
#include "RHI/RHIGlobal.h"
#include "RHI/RHIGraphics.h"

NTexture2DTaskRenderAssetProxy::NTexture2DTaskRenderAssetProxy(Task<std::shared_ptr<NRHITexture2D>> InTextureTask)
	: TextureTask(std::move(InTextureTask))
{
	SRV = NRHIGlobal::GetDynamicRHI().CreateShaderResourceView(1);
	SRV->CreateView(0, nullptr);
}

NTexture2DTaskRenderAssetProxy::~NTexture2DTaskRenderAssetProxy() noexcept
{
}

bool NTexture2DTaskRenderAssetProxy::TryResolve()
{
	if (TextureTask.IsValid())
	{
		if (TextureTask.IsCompleted())
		{
			Texture = TextureTask.GetResult();
			SRV->CreateView(0, Texture.get());
			return true;
		}
		return false;
	}
	return true;
}

std::shared_ptr<NRHITexture2D> NTexture2DTaskRenderAssetProxy::GetTexture2D() const
{
	return Texture;
}

std::shared_ptr<NRHIShaderResourceView> NTexture2DTaskRenderAssetProxy::GetSRV() const
{
	return SRV;
}

Rect NTexture2DTaskRenderAssetProxy::GetTextureCoordinate() const
{
	return Rect(0.0f, 0.0f, 1.0f, 1.0f);
}

int32 NTexture2DTaskRenderAssetProxy::GetRenderStates() const
{
	return RS_HasTexture;
}