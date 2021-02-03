// Copyright 2020 Aumoa.lib. All right reserved.

#include "RHI/RHITextureClearValue.h"

#include "RHI/RHITextureFormat.h"

RHITextureClearValue::RHITextureClearValue()
	: Format(ERHITextureFormat::Unknown)
{

}

RHITextureClearValue::RHITextureClearValue(ERHITextureFormat inFormat)
	: Format(inFormat)
{

}

bool RHITextureClearValue::IsValid_get() const
{
	if (IsUnknownFormat(Format))
	{
		return false;
	}

	return true;
}

bool RHITextureClearValue::IsColor_get() const
{
	return IsColorFormat(Format);
}

bool RHITextureClearValue::IsDepthStencil_get() const
{
	return IsDepthStencilFormat(Format);
}

bool RHITextureClearValue::IsUnknownFormat(ERHITextureFormat inFormat)
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

bool RHITextureClearValue::IsColorFormat(ERHITextureFormat inFormat)
{
	if (inFormat == ERHITextureFormat::R8G8B8A8_UNORM)
	{
		return true;
	}
	else if (inFormat == ERHITextureFormat::B8G8R8A8_UNORM)
	{
		return true;
	}

	return false;
}

bool RHITextureClearValue::IsDepthStencilFormat(ERHITextureFormat inFormat)
{
	if (inFormat == ERHITextureFormat::D24_UNORM_S8_UINT)
	{
		return true;
	}

	return false;
}