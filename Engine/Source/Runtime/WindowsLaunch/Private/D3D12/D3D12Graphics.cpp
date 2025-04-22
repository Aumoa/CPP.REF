// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "D3D12/D3D12Graphics.h"

namespace Ayla
{
	void D3D12Graphics::Initialize()
	{
		HR(::D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&m_Device)));
	}
}