// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "D3D12RHI/D3D12Texture2D.h"

#if PLATFORM_WINDOWS

ND3D12Texture2D::ND3D12Texture2D(ComPtr<ID3D12Resource> InResource, const D3D12_RESOURCE_DESC& InDesc)
	: Resource(std::move(InResource))
	, Desc(InDesc)
{
}

ND3D12Texture2D::~ND3D12Texture2D() noexcept
{
}

Vector2N ND3D12Texture2D::GetTextureSize() const
{
	return Vector2N((int32)Desc.Width, (int32)Desc.Height);
}

bool ND3D12Texture2D::IsRenderTarget() const
{
	return (Desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET) > 0;
}

#endif