// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Assets/Texture2D.h"
#include "GameEngine.h"
#include "EngineSubsystems/GameRenderSystem.h"
#include "Multimedia/IPlatformImage.h"
#include "Multimedia/IPlatformImageLoader.h"
#include "RHI/IRHIDevice.h"
#include "RHI/IRHITexture2D.h"
#include "RHI/IRHIBitmap.h"
#include "IApplicationInterface.h"

STexture2D::STexture2D(const std::filesystem::path& assetPath) : Super(assetPath)
{
}

STexture2D::~STexture2D()
{
}

void STexture2D::StreamIn()
{
	auto& Application = IApplicationInterface::Get();
	IPlatformImageLoader& Loader = Application.GetPlatformImageLoader();

	IPlatformImage* Image = Loader.CreateImageFromFile(GetAssetPath(), 0, ERHIPixelFormat::B8G8R8A8_UNORM);
	Image->SetOuter(this);

	auto* RenderSystem = GEngine->GetEngineSubsystem<SGameRenderSystem>();
	IRHIDevice* Device = RenderSystem->GetRHIDevice();

	// Copy pixels to buffer.
	Vector2N PixelSize = Image->GetPixelSize();
	std::vector<uint8> PixelsData((size_t)PixelSize.Size() * 4);
	int32 Stride = PixelSize.X * 4;
	Image->CopyPixels(nullptr, Stride, (int32)PixelsData.size(), PixelsData.data());

	// Create texture data.
	RHITexture2DDesc Desc = {};
	Desc.Width = PixelSize.X;
	Desc.Height = PixelSize.Y;
	Desc.DepthOrArraySize = 1;
	Desc.MipLevels = 1;
	Desc.Format = ERHIPixelFormat::B8G8R8A8_UNORM;
	Desc.Usage = ERHIBufferUsage::Immutable;
	Desc.InitialState = ERHIResourceStates::PixelShaderResource;
	Desc.SampleDesc = { 1, 0 };

	RHISubresourceData InitialData;
	InitialData.pSysMem = PixelsData.data();
	InitialData.SysMemPitch = (size_t)PixelSize.X * 4;

	Texture = Device->CreateTexture2D(Desc, &InitialData);
	Texture->SetOuter(this);

	Bitmap = Device->CreateBitmapFromTexture2D(Texture);
	Bitmap->SetOuter(this);
}

Vector2N STexture2D::GetPixelSize()
{
	if (Texture)
	{
		RHITexture2DDesc Desc = Texture->GetDesc();
		return Vector2N((int32)Desc.Width, (int32)Desc.Height);
	}
	else
	{
		return Vector2N::ZeroVector();
	}
}