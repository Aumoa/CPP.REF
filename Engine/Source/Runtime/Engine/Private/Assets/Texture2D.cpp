// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Assets/Texture2D.h"
#include "GenericPlatform/GenericImage.h"
#include "RenderGlobal.h"
#include "RHI/RHITexture2D.h"
#include "RHI/RHIGlobal.h"
#include "RHI/RHIGraphics.h"

bool ATexture2D::IsStreamedIn() const
{
	return RenderProxy && RenderProxy->TryResolve();
}

void ATexture2D::StreamIn()
{
	if (!RenderProxy)
	{
		RenderProxy = NRenderGlobal::LoadTexture2DAsync(AbsolutePath);
	}
}

void ATexture2D::StreamOut()
{
	RenderProxy.reset();
}

std::shared_ptr<NStreamableRenderAssetProxy> ATexture2D::GetRenderProxy() const
{
	return RenderProxy;
}

RefPtr<ATexture2D> ATexture2D::LoadAndStreamIn(String InAbsolutePath)
{
	auto Instance = ::NewObject<ATexture2D>();
	Instance->AbsolutePath = InAbsolutePath;
	Instance->StreamIn();
	return Instance;
}