// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Texture/Texture2D.h"
#include "LogAssetsIO.h"
#include "IApplicationInterface.h"
#include "AssetsLoader.h"
#include "Multimedia/IPlatformImageLoader.h"
#include "Multimedia/IPlatformImage.h"
#include "RHI/IRHITexture2D.h"
#include "RHI/IRHIBitmap.h"
#include "RHI/IRHIDevice.h"

#pragma pack(push, 1)

struct Texture2DHeader
{
	uint32 Version;
};

#pragma pack(pop)

STexture2D::STexture2D() : Super()
{
}

bool STexture2D::StreamIn(SAssetsLoader* Loader, std::span<const uint8> AssetsBin)
{
	const uint8* Seek = AssetsBin.data();

	Texture2DHeader Header;
	memcpy(&Header, Seek, sizeof(Header));
	Seek += sizeof(Header);

	if (Header.Version == 0 || Header.Version > ImporterVersion)
	{
		SE_LOG(LogAssetsIO, Error, L"Assets version mismatch. AssetVersion: {}, ImporterVersion: {}", Header.Version, ImporterVersion);
		return false;
	}

	std::span<const uint8> Body(Seek, AssetsBin.size() - sizeof(Header));

	IPlatformImageLoader& ImageLoader = IApplicationInterface::Get().GetPlatformImageLoader();
	ImageSource = ImageLoader.CreateImageFromBinary(Body, 0, ERHIPixelFormat::B8G8R8A8_UNORM);
	if (ImageSource)
	{
		ImageSource->SetOuter(this);
		IRHIDevice* Device = Loader->GetDevice();

		// Copy pixels to buffer.
		Vector2N PixelSize = ImageSource->GetPixelSize();
		std::vector<uint8> PixelsData((size_t)PixelSize.Size() * 4);
		int32 Stride = PixelSize.X * 4;
		ImageSource->CopyPixels(nullptr, Stride, (int32)PixelsData.size(), PixelsData.data());

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

	return ImageSource != nullptr;
}

std::vector<uint8> STexture2D::StreamOut(SAssetsLoader* Loader, std::span<const uint8> AssetsBin)
{
	Texture2DHeader Header;
	Header.Version = ImporterVersion;

	std::vector<uint8> Body(sizeof(Texture2DHeader) + AssetsBin.size());
	uint8* Seek = Body.data();

	memcpy(Seek, &Header, sizeof(Header));
	Seek += sizeof(Header);

	memcpy(Seek, AssetsBin.data(), AssetsBin.size_bytes());
	Seek += AssetsBin.size_bytes();

	return Body;
}

Vector2N STexture2D::GetSize()
{
	return ImageSource->GetPixelSize();
}

IPlatformImage* STexture2D::GetPlatformImage()
{
	return ImageSource;
}

IRHITexture2D* STexture2D::GetTexture()
{
	return Texture;
}

IRHIBitmap* STexture2D::GetBitmap()
{
	return Bitmap;
}