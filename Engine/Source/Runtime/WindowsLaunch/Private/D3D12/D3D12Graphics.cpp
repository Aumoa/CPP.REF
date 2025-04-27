// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "D3D12/D3D12Graphics.h"
#include "D3D12/D3D12Window.h"
#include "GenericPlatform/GenericWindow.h"

namespace Ayla
{
	void D3D12Graphics::Initialize()
	{
		UINT dxgiCreateFlags = 0;
#if DO_CHECK
		dxgiCreateFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif
		HR(::CreateDXGIFactory2(dxgiCreateFlags, IID_PPV_ARGS(&m_DXGI)));

		HR(::D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&m_Device)));

		D3D12_COMMAND_QUEUE_DESC commandQueueDesc =
		{
			.Type = D3D12_COMMAND_LIST_TYPE_DIRECT,
			.Priority = D3D12_COMMAND_QUEUE_PRIORITY_HIGH,
			.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE,
			.NodeMask = 0
		};
		HR(m_Device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&m_PrimaryCommandQueue)));
	}

	RPtr<Window> D3D12Graphics::ConfigureWindow(RPtr<GenericWindow> platformWindow)
	{
		auto window = New<D3D12Window>();
		window->Initialize(rthis, platformWindow);
		return window;
	}
}