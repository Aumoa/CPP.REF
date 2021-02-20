// Copyright 2020-2021 Aumoa.lib. All right reserved.

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