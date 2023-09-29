// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Rendering/Texture2DTaskRenderProxy.h"
#include "RHI/RHITexture2D.h"
#include "RHI/RHIShaderResourceView.h"
#include "RHI/RHIGlobal.h"
#include "RHI/RHIGraphics.h"

NTexture2DTaskRenderProxy::NTexture2DTaskRenderProxy(Task<std::shared_ptr<NRHITexture2D>> InTextureTask)
	: TextureTask(std::move(InTextureTask))
{
	SRV = NRHIGlobal::GetDynamicRHI().CreateShaderResourceView(1);
	SRV->CreateView(0, nullptr);
}

NTexture2DTaskRenderProxy::~NTexture2DTaskRenderProxy() noexcept
{
}

bool NTexture2DTaskRenderProxy::TryResolve()
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

std::shared_ptr<NRHITexture2D> NTexture2DTaskRenderProxy::GetTexture2D() const
{
	return Texture;
}

std::shared_ptr<NRHIShaderResourceView> NTexture2DTaskRenderProxy::GetSRV() const
{
	return SRV;
}

Rect NTexture2DTaskRenderProxy::GetTextureCoordinate() const
{
	return Rect(0.0f, 0.0f, 1.0f, 1.0f);
}

int32 NTexture2DTaskRenderProxy::GetRenderStates() const
{
	return RS_HasTexture;
}