// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Assets/Texture2D.h"
#include "GameEngine.h"
#include "EngineSubsystems/GameRenderSystem.h"
#include "PlatformMisc/IPlatformImageLoader.h"
#include "PlatformMisc/IPlatformImage.h"

Texture2D::Texture2D(const std::filesystem::path& assetPath) : Super(assetPath)
{
	_shaderResourceView = NewObject<RHIShaderResourceView>(GEngine->GetEngineSubsystem<GameRenderSystem>()->GetRHIDevice(), 1);
}

Texture2D::~Texture2D()
{
}

void Texture2D::StreamIn()
{
	auto* renderSystem = GEngine->GetEngineSubsystem<GameRenderSystem>();
	RHIDevice* device = renderSystem->GetRHIDevice();
	IPlatformImageLoader& imageLoader = IPlatformImageLoader::Get();

	ScopedPtr<IPlatformImage> platformImage = imageLoader.LoadImageFromFile(GetAssetPath(), ERHIPixelFormat::B8G8R8A8_UNORM);
	_texture = device->CreateTexture2DFromImage(platformImage.Get());
	_shaderResourceView->CreateShaderResourceView(_texture, 0);
}

void Texture2D::GetPixelSize(int32* pWidth, int32* pHeight) const
{
	if (_texture)
	{
		_texture->GetPixelSize(pWidth, pHeight);
	}
	else
	{
		if (pWidth)
		{
			*pWidth = 0;
		}
		if (pHeight)
		{
			*pHeight = 0;
		}
	}
}

Vector2 Texture2D::GetSize() const
{
	int32 width, height;
	GetPixelSize(&width, &height);
	return Vector2((float)width, (float)height);
}