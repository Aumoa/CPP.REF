// Copyright 2020 Aumoa.lib. All right reserved.

#include "Logging/LogMacros.h"
#include "RHI/RHICommon.h"

inline bool operator <(const WICPixelFormatGUID& lh, const WICPixelFormatGUID& rh)
{
	return memcmp(&lh, &rh, sizeof(lh)) < 0;
}

extern ComPtr<IWICImagingFactory2> imagingFactory;
static std::tuple<ERHITextureFormat, WICPixelFormatGUID, size_t> convertTable[]
{
	{ ERHITextureFormat::R16G16B16A16_UNORM, GUID_WICPixelFormat64bppRGBA, 64 },
	{ ERHITextureFormat::B8G8R8A8_UNORM, GUID_WICPixelFormat32bppPBGRA, 32 },
	{ ERHITextureFormat::R8G8B8A8_UNORM, GUID_WICPixelFormat32bppPRGBA, 32 },
	{ ERHITextureFormat::Unknown, GUID_WICPixelFormat24bppBGR, 24 },
	{ ERHITextureFormat::Unknown, GUID_WICPixelFormat24bppRGB, 24 },
};
static constexpr size_t convertTable_ArraySize = sizeof(convertTable) / sizeof(convertTable[0]);

template<size_t... index>
inline std::map<ERHITextureFormat, WICPixelFormatGUID> GetWICFormatImpl(std::index_sequence<index...>&&)
{
	return { { get<0>(convertTable[index]), get<1>(convertTable[index]) }... };
}

template<size_t... index>
inline std::map<WICPixelFormatGUID, size_t> CalcBitsPerPixelImpl(std::index_sequence<index...>&&)
{
	return { { get<1>(convertTable[index]), get<2>(convertTable[index]) }... };
}

#define GetImpl(x) x ## Impl(std::make_index_sequence<convertTable_ArraySize>{ })

inline size_t CalcBitsPerPixel(const WICPixelFormatGUID& formatGuid)
{
	static std::map<WICPixelFormatGUID, size_t> ConvertTable = GetImpl(CalcBitsPerPixel);

	auto it = ConvertTable.find(formatGuid);
	if (it == ConvertTable.end())
	{
		SE_LOG(LogPlatform, Error, L"GUID format not supports.");
		return 0;
	}

	return it->second;
}

inline WICPixelFormatGUID GetWICFormat(ERHITextureFormat inFormat)
{
	static std::map<ERHITextureFormat, WICPixelFormatGUID> ConvertTable = GetImpl(GetWICFormat);

	auto it = ConvertTable.find(inFormat);
	if (it == ConvertTable.end())
	{
		SE_LOG(LogPlatform, Error, L"GUID format not supports.");
		return GUID_WICPixelFormatUndefined;
	}

	return it->second;
}