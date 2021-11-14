// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "AssetsLoader.h"
#include "LogAssetsIO.h"
#include "IAssets.h"
#include "Texture/Texture2D.h"

#pragma pack(push, 1)

enum class EAssetType : uint8
{
	Image,
};

struct GenericAssetHeader
{
	uint32 Version;
	EAssetType Type;
};

#pragma pack(pop)

class ScopedFile
{
	std::ifstream Fp;

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
};

SAssetsLoader::SAssetsLoader(IRHIDevice* InDevice) : Super()
	, Device(InDevice)
{
}

SObject* SAssetsLoader::ImportFromFile(const std::filesystem::path& AssetPath)
{
#define ASSETS_LOG(Message) SE_LOG(LogAssetsIO, Error, L"Could not import assets from file '{}': {}", AssetPath.wstring(), Message)

	static constexpr size_t HeaderSize = sizeof(GenericAssetHeader);
	ScopedFile Fp(AssetPath, std::ios::binary | std::ios::in);
	if (Fp->is_open())
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
	Fp->read((char*)&Header, HeaderSize);

	if (Header.Version == 0 && Header.Version > ImporterVersion)
	{
		ASSETS_LOG(std::format(L"Not supported import version. FileVersion: {}, ImporterVersion: {}", Header.Version, ImporterVersion));
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

	IAssets* Asset = nullptr;
	switch (Header.Type)
	{
	case EAssetType::Image:
		Asset = NewObject<STexture2D>();
		break;
	default:
		ASSETS_LOG(std::format(L"Item type mismatch. Type: (EAssetType){}", (int32)Header.Type));
		return nullptr;
	}

	if (Asset == nullptr || !Asset->StreamIn(this, AssetBinary))
	{
		ASSETS_LOG(L"Could not parsing assets binary.");
		return nullptr;
	}

	return Asset;
}

bool SAssetsLoader::ConvertAssets(const std::filesystem::path& NativeAssetPath, const std::filesystem::path& ConvertPath)
{
	// TODO:
	check(false);
	return false;
}

IRHIDevice* SAssetsLoader::GetDevice()
{
	return Device;
}