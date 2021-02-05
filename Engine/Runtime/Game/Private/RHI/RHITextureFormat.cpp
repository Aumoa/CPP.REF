// Copyright 2020 Aumoa.lib. All right reserved.

#include "RHI/RHITextureFormat.h"

bool IsUnknownFormat(ERHITextureFormat inFormat)
{
	if (inFormat == ERHITextureFormat::Unknown)
	{
		return true;
	}
	else if (inFormat == ERHITextureFormat::R24G8_TYPELESS)
	{
		return true;
	}

	return false;
}

bool IsColorFormat(ERHITextureFormat inFormat)
{
	if (inFormat == ERHITextureFormat::R16G16B16A16_FLOAT)
	{
		return true;
	}
	else if (inFormat == ERHITextureFormat::R8G8B8A8_UNORM)
	{
		return true;
	}
	else if (inFormat == ERHITextureFormat::B8G8R8A8_UNORM)
	{
		return true;
	}

	return false;
}

bool IsDepthStencilFormat(ERHITextureFormat inFormat)
{
	if (inFormat == ERHITextureFormat::D24_UNORM_S8_UINT)
	{
		return true;
	}

	return false;
}