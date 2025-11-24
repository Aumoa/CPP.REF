// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "D3D12Graphics.h"

namespace Ayla
{
	D3D12Graphics::D3D12Graphics()
	{
		UINT dxgiFactoryFlags = 0;
#if DO_CHECK
		dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif

		HR(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&m_DXGI)));
	}

	D3D12Graphics::~D3D12Graphics() noexcept
	{
	}
}