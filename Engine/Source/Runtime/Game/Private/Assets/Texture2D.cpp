// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Assets/Texture2D.h"
#include "GameEngine.h"
#include "EngineSubsystems/GameRenderSystem.h"
#include "PlatformMisc/IPlatformImageLoader.h"
#include "PlatformMisc/IPlatformImage.h"
#include "RHI/IRHIDevice.h"
#include "RHI/IRHITexture2D.h"

STexture2D::STexture2D(const std::filesystem::path& assetPath) : Super(assetPath)
{
}

STexture2D::~STexture2D()
{
}

void STexture2D::StreamIn()
{
	auto* renderSystem = GEngine->GetEngineSubsystem<SGameRenderSystem>();
	IRHIDevice* device = renderSystem->GetRHIDevice();
	IPlatformImageLoader& imageLoader = IPlatformImageLoader::Get();

	// Load image from file.
	ScopedPtr<IPlatformImage> platformImage = imageLoader.LoadImageFromFile(GetAssetPath(), ERHIPixelFormat::B8G8R8A8_UNORM);
	Vector2N pixelSize = platformImage->GetPixelSize();
	std::vector<uint8> imagePixels(pixelSize.Size() * 4);

	// Copy pixels data.
	platformImage->CopyPixels(4 * (uint32)pixelSize.X, (uint32)imagePixels.size(), imagePixels.data());

	RHITexture2DDesc textureDesc = {};
	textureDesc.Width = pixelSize.X;
	textureDesc.Height = pixelSize.Y;
	textureDesc.DepthOrArraySize = 1;
	textureDesc.MipLevels = 1;
	textureDesc.Format = ERHIPixelFormat::B8G8R8A8_UNORM;
	textureDesc.Usage = ERHIBufferUsage::Immutable;
	textureDesc.InitialState = ERHIResourceStates::PixelShaderResource;

	RHISubresourceData initialData;
	initialData.pSysMem = imagePixels.data();
	initialData.SysMemPitch = pixelSize.X * 4;

	_texture = device->CreateTexture2D(textureDesc, &initialData);
	_texture->SetOuter(this);
}

Vector2N STexture2D::GetPixelSize()
{
	if (_texture)
	{
		RHITexture2DDesc desc = _texture->GetDesc();
		return Vector2N((int32)desc.Width, (int32)desc.Height);
	}
	else
	{
		return Vector2N::ZeroVector();
	}
}