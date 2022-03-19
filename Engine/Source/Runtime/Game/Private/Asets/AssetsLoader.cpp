// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Assets/AssetsLoader.h"
#include "Assets/Texture/Texture2D.h"
#include "LogGame.h"
#include <fstream>

GENERATE_BODY(SAssetsLoader);

#pragma pack(push, 1)

SENUM(EAssetType, uint8,
	Image,
);

struct GenericAssetHeader
{
	uint32 Version;
	EAssetType Type;
};

#pragma pack(pop)

class ScopedFile
{
	std::fstream Fp;

public:
	template<class... TArgs>
	ScopedFile(TArgs&&... InArgs) : Fp(std::forward<TArgs>(InArgs)...)
	{
	}

	~ScopedFile()
	{
		if (Fp.is_open())
		{
			Fp.close();
		}
	}

	auto operator ->()
	{
		return &Fp;
	}

	auto& operator *()
	{
		return Fp;
	}
};

SAssetsLoader::SAssetsLoader(IRHIDevice* InDevice) : Super()
	, Device(InDevice)
{
}

SObject* SAssetsLoader::ImportFromFile(const std::filesystem::path& AssetPath)
{
#define ASSETS_LOG(Message) SE_LOG(LogAssets, Error, L"Could not import assets from file '{}': {}", AssetPath.wstring(), Message)

	static constexpr size_t HeaderSize = sizeof(GenericAssetHeader);
	ScopedFile Fp(AssetPath, std::ios::binary | std::ios::in);
	if (!Fp->is_open())
	{
		ASSETS_LOG(L"Unable to access file.");
		return nullptr;
	}

	size_t Size = Fp->seekg(0, std::ios::end).tellg();
	if (Size < HeaderSize)
	{
		ASSETS_LOG(L"File is not compatible.");
		return nullptr;
	}

	GenericAssetHeader Header;
	if (Fp->seekg(0, std::ios::beg).read((char*)&Header, HeaderSize).bad())
	{
		ASSETS_LOG(String::Format(L"Unable to access file."));
		return nullptr;
	}

	if (Header.Version == 0 && Header.Version > ImporterVersion)
	{
		ASSETS_LOG(String::Format(L"Not supported import version. FileVersion: {}, ImporterVersion: {}", Header.Version, ImporterVersion));
		return nullptr;
	}

	std::vector<uint8> AssetBinary(Size - HeaderSize);
	Fp->read((char*)AssetBinary.data(), AssetBinary.size());

	if (Fp->bad())
	{
		ASSETS_LOG(L"Unable to access file.");
		return nullptr;
	}

	Fp->close();

	SStreamableRenderAsset* Asset = nullptr;
	switch (Header.Type)
	{
	case EAssetType::Image:
		Asset = gcnew STexture2D();
		break;
	default:
		ASSETS_LOG(String::Format(L"Item type mismatch. Type: (EAssetType){}", (int32)Header.Type));
		return nullptr;
	}

	if (Asset == nullptr || !Asset->StreamIn(this, AssetBinary))
	{
		ASSETS_LOG(L"Could not parsing assets binary.");
		return nullptr;
	}

#undef ASSETS_LOG

	return Asset;
}

bool SAssetsLoader::ConvertAssets(const std::filesystem::path& NativeAssetPath, const std::filesystem::path& ConvertPath)
{
	static std::set<std::filesystem::path> ImageExt = { L".jpg", L".jpeg", L".png", L".bmp", L".gif" };
	std::filesystem::path Ext = NativeAssetPath.extension();

	ScopedFile FpSrc(NativeAssetPath, std::ios::binary | std::ios::in);

	if (!FpSrc->is_open())
	{
		return false;
	}

	if (ImageExt.contains(Ext))
	{
		ScopedFile FpDst(ConvertPath, std::ios::binary | std::ios::out | std::ios::trunc);
		return StreamOut_Texture2D(*FpSrc, *FpDst);
	}

	return false;
}

IRHIDevice* SAssetsLoader::GetDevice()
{
	return Device;
}

bool SAssetsLoader::StreamOut_Texture2D(std::fstream& Source, std::fstream& FileStream)
{
	size_t BodySize = Source.seekg(0, std::ios::end).tellg();
	std::vector<uint8> Body(BodySize);
	if (Source.seekg(0, std::ios::beg).read((char*)Body.data(), BodySize).bad())
	{
		return false;
	}

	std::vector<uint8> PackedImage = STexture2D::StreamOut(this, Body);

	GenericAssetHeader Header;
	Header.Version = ImporterVersion;
	Header.Type = EAssetType::Image;
	if (FileStream.write((const char*)&Header, sizeof(Header)).bad())
	{
		return false;
	}

	if (FileStream.write((const char*)PackedImage.data(), PackedImage.size()).bad())
	{
		return false;
	}

	FileStream.flush();
	return true;
}