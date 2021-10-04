// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DXGIFactory.h"
#include "DXGIAdapter.h"
#include "DXGISwapChain.h"
#include "IWindowView.h"
#include "RHI/D3D12/D3D12Device.h"
#include "RHI/D3D12/D3D12CommandQueue.h"
#include "RHI/IRHIDeviceContext.h"

SDXGIFactory::SDXGIFactory() : Super()
{
	UINT flags = 0;
#if defined(_DEBUG)
	flags |= DXGI_CREATE_FACTORY_DEBUG;
#endif

	HR(CreateDXGIFactory2(flags, IID_PPV_ARGS(&_factory)));
}

IRHIAdapter* SDXGIFactory::GetAdapter(int32 index)
{
	if (_cachedAdapters.size() > index && _cachedAdapters[index] != nullptr)
	{
		return _cachedAdapters[index];
	}

	ComPtr<IDXGIAdapter1> adapter;
	HRESULT hr = _factory->EnumAdapters1((UINT)index, &adapter);
	if (hr == DXGI_ERROR_NOT_FOUND)
	{
		return nullptr;
	}
	else
	{
		HR(("_factory->EnumAdapters1", hr));
	}

	auto* instance = NewObject<SDXGIAdapter>(this, std::move(adapter));
	if (_cachedAdapters.size() <= (size_t)index)
	{
		_cachedAdapters.resize((size_t)index + 1);
	}
	_cachedAdapters[index] = instance;
	return instance;
}

IRHIDevice* SDXGIFactory::CreateDevice(IRHIAdapter* adapter)
{
	auto instance = Cast<SDXGIAdapter>(adapter);
	IDXGIAdapter1* actualAdapter = instance->GetAdapter();

	ComPtr<ID3D12Device> device;
	HR(D3D12CreateDevice(actualAdapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device)));

	return NewObject<SD3D12Device>(this, std::move(device));
}

IRHISwapChain* SDXGIFactory::CreateSwapChain(IWindowView* window, IRHIDevice* device)
{
	HWND wHandle = reinterpret_cast<HWND>(window->GetWindowHandle());

	DXGI_SWAP_CHAIN_DESC1 desc1 =
	{
		.Format = DXGI_FORMAT_B8G8R8A8_UNORM,
		.SampleDesc = { 1, 0 },
		.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
		.BufferCount = 3,
		.Scaling = DXGI_SCALING_STRETCH,
		.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
		.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED
	};

	auto primaryQueue = Cast<SD3D12CommandQueue>(device->GetImmediateContext());

	ComPtr<IDXGISwapChain1> swapChain1;
	HR(_factory->CreateSwapChainForHwnd(primaryQueue->Get<ID3D12CommandQueue>(), wHandle, &desc1, nullptr, nullptr, &swapChain1));

	ComPtr<IDXGISwapChain4> swapChain4;
	HR(swapChain1.As<IDXGISwapChain4>(&swapChain4));

	return NewObject<SDXGISwapChain>(this, Cast<SD3D12Device>(device), std::move(swapChain4));
}