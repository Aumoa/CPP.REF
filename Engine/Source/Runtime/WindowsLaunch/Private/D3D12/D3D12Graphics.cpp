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

		UINT d3d11CreateDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if DO_CHECK
		d3d11CreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		HR(::D3D11On12CreateDevice(m_Device.Get(), d3d11CreateDeviceFlags, NULL, 0, (IUnknown**)m_PrimaryCommandQueue.GetAddressOf(), 1, 0, &m_Device11, &m_DeviceContext11, NULL));
		HR(m_Device11.As(&m_Device11On12));
		ComPtr<IDXGIDevice> dxgiDevice;
		HR(m_Device11.As(&dxgiDevice));

		D2D1_DEBUG_LEVEL d2d1DebugLevel = D2D1_DEBUG_LEVEL_NONE;
#if DO_CHECK
		d2d1DebugLevel = D2D1_DEBUG_LEVEL_WARNING;
#endif

		HR(::D2D1CreateDevice(dxgiDevice.Get(), D2D1::CreationProperties(D2D1_THREADING_MODE_SINGLE_THREADED, d2d1DebugLevel, D2D1_DEVICE_CONTEXT_OPTIONS_NONE), &m_Device2D));
	}

	RPtr<Window> D3D12Graphics::ConfigureWindow(RPtr<GenericWindow> platformWindow)
	{
		auto window = New<D3D12Window>();
		window->Initialize(rthis, platformWindow);
		return window;
	}
}