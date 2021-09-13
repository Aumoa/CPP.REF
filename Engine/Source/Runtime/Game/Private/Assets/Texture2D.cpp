// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Assets/Texture2D.h"
#include "GameEngine.h"
#include "EngineSubsystems/GameRenderSystem.h"
#include "PlatformMisc/IPlatformImageLoader.h"
#include "PlatformMisc/IPlatformImage.h"

STexture2D::STexture2D(const std::filesystem::path& assetPath) : Super(assetPath)
{
	_shaderResourceView = NewObject<SRHIShaderResourceView>(GEngine->GetEngineSubsystem<SGameRenderSystem>()->GetRHIDevice(), 1);
}

STexture2D::~STexture2D()
{
}

void STexture2D::StreamIn()
{
	auto* renderSystem = GEngine->GetEngineSubsystem<SGameRenderSystem>();
	SRHIDevice* device = renderSystem->GetRHIDevice();
	IPlatformImageLoader& imageLoader = IPlatformImageLoader::Get();

	ScopedPtr<IPlatformImage> platformImage = imageLoader.LoadImageFromFile(GetAssetPath(), ERHIPixelFormat::B8G8R8A8_UNORM);
	_texture = device->CreateTexture2DFromImage(platformImage.Get());
	_shaderResourceView->CreateShaderResourceView(_texture, 0);
}

void STexture2D::GetPixelSize(int32* pWidth, int32* pHeight) const
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

Vector2 STexture2D::GetSize() const
{
	int32 width, height;
	GetPixelSize(&width, &height);
	return Vector2((float)width, (float)height);
}