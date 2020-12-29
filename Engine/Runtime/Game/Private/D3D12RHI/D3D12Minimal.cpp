// Copyright 2020 Aumoa.lib. All right reserved.

#include "D3D12Minimal.h"

#include "Logging/LogVerbosity.h"

LogCategoryBase LogD3D12RHI(ELogVerbosity::Verbose, nameof(LogD3D12RHI));

D3D12_RESOURCE_STATES ToD3D12(RHIResourceStates value)
{
	switch (value)
	{
	case RHIResourceStates::PRESENT:
		return D3D12_RESOURCE_STATE_PRESENT;
	case RHIResourceStates::RENDER_TARGET:
		return D3D12_RESOURCE_STATE_RENDER_TARGET;
	case RHIResourceStates::COPY_SOURCE:
		return D3D12_RESOURCE_STATE_COPY_SOURCE;
	case RHIResourceStates::COPY_DEST:
		return D3D12_RESOURCE_STATE_COPY_DEST;
	default:
		return D3D12_RESOURCE_STATE_COMMON;
	}
}

DXGI_FORMAT ToD3D12(RHITextureFormat value)
{
	switch (value)
	{
	case RHITextureFormat::R8G8B8A8_UNORM:
		return DXGI_FORMAT_R8G8B8A8_UNORM;
	default:
		return DXGI_FORMAT_UNKNOWN;
	}
}

D3D12_RESOURCE_FLAGS ToD3D12(RHIResourceFlags value)
{
	bool bAllowRenderTarget = (value & RHIResourceFlags::AllowRenderTarget) != RHIResourceFlags::None;
	D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE;
	if (bAllowRenderTarget)
	{
		flags = flags | D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
	}
	return flags;
}

RHIResourceStates ToRHI(D3D12_RESOURCE_STATES value)
{
	switch (value)
	{
	case D3D12_RESOURCE_STATE_PRESENT:
		return RHIResourceStates::PRESENT;
	case D3D12_RESOURCE_STATE_RENDER_TARGET:
		return RHIResourceStates::RENDER_TARGET;
	case D3D12_RESOURCE_STATE_COPY_SOURCE:
		return RHIResourceStates::COPY_SOURCE;
	case D3D12_RESOURCE_STATE_COPY_DEST:
		return RHIResourceStates::COPY_DEST;
	default:
		return RHIResourceStates::PRESENT;
	}
}

RHITextureFormat ToRHI(DXGI_FORMAT value)
{
	switch (value)
	{
	case DXGI_FORMAT_R8G8B8A8_UNORM:
		return RHITextureFormat::R8G8B8A8_UNORM;
	default:
		return RHITextureFormat::Unknown;
	}
}

RHIResourceFlags ToRHI(D3D12_RESOURCE_FLAGS value)
{
	bool bAllowRenderTarget = (value & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET) != D3D12_RESOURCE_FLAG_NONE;
	RHIResourceFlags flags = RHIResourceFlags::None;
	if (bAllowRenderTarget)
	{
		flags = flags | RHIResourceFlags::AllowRenderTarget;
	}
	return flags;
}

bool IsDepthStencilFormat(RHITextureFormat value)
{
	return false;
}