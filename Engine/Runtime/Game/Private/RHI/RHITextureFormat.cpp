// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "RHI/RHITextureFormat.h"

#define CASE(x) case ERHITextureFormat::x

bool IsUnknownFormat(ERHITextureFormat inFormat)
{
	switch (inFormat)
	{
	CASE(Unknown):
	CASE(R24G8_TYPELESS):
	CASE(R24_UNORM_X8_TYPELESS):
	CASE(X24_UNORM_G8_UINT):
		return true;
	}

	return false;
}

bool IsColorFormat(ERHITextureFormat inFormat)
{
	if (inFormat >= ERHITextureFormat::R16G16B16A16_FLOAT && inFormat <= ERHITextureFormat::R16G16B16A16_SINT)
	{
		return true;
	}

	switch (inFormat)
	{
	CASE(R8G8B8A8_UNORM):
	CASE(B8G8R8A8_UNORM):
		return true;
	}

	return false;
}

bool IsDepthStencilFormat(ERHITextureFormat inFormat)
{
	switch (inFormat)
	{
	CASE(D24_UNORM_S8_UINT):
		return true;
	}

	return false;
}