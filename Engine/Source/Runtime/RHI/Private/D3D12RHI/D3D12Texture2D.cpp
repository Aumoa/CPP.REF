// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "D3D12RHI/D3D12Texture2D.h"

#if PLATFORM_WINDOWS

ND3D12Texture2D::ND3D12Texture2D(ComPtr<ID3D12Resource> InResource)
	: Resource(std::move(InResource))
{
}

ND3D12Texture2D::~ND3D12Texture2D() noexcept
{
}

#endif