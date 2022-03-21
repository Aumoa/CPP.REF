// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "DirectXFactory.h"
#include "DirectXAdapter.h"
#include "DirectXCommandQueue.h"
#include "DirectXSwapChain.h"
#include "DirectXDevice.h"
#include "IApplicationInterface.h"

GENERATE_BODY(SDirectXFactory);

SDirectXFactory::SDirectXFactory(bool bEnableDebugLayer)
	: Super()
{
	HR(CreateDXGIFactory1(IID_PPV_ARGS(&pFactory)));
	
	if (bEnableDebugLayer)
	{
		if (ComPtr<ID3D12Debug> pDebug; SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&pDebug))))
		{
			pDebug->EnableDebugLayer();
		}
	}
}

void SDirectXFactory::Dispose()
{
	Dispose(true);
}

IRHIAdapter* SDirectXFactory::GetAdapter(size_t index)
{
	ComPtr<IDXGIAdapter1> pAdapter;
	HR(pFactory->EnumAdapters1((UINT)index, &pAdapter));

	if (pAdapter)
	{
		return gcnew SDirectXAdapter(this, std::move(pAdapter));
	}
	else
	{
		return nullptr;
	}
}

IRHIDevice* SDirectXFactory::CreateDevice(IRHIAdapter* pAdapter)
{
	auto sAdapter = Cast<SDirectXAdapter>(pAdapter);

	ComPtr<ID3D12Device> pDevice;
	HR(D3D12CreateDevice(sAdapter->pAdapter.Get(), D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&pDevice)));

	return gcnew SDirectXDevice(this, std::move(pDevice));
}

IRHISwapChain* SDirectXFactory::CreateSwapChain(IRHICommandQueue* pQueue, size_t numBuffers)
{
	Vector2N Size = IApplicationInterface::Get().GetViewportSize();
	DXGI_SWAP_CHAIN_DESC1 SwapChainDesc =
	{
		.Width = (UINT)Size.X,
		.Height = (UINT)Size.Y,
		.Format = DXGI_FORMAT_B8G8R8A8_UNORM,
		.SampleDesc = { 1, 0 },
		.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
		.BufferCount = (UINT)numBuffers,
		.Scaling = DXGI_SCALING_STRETCH,
		.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
		.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED
	};

	auto sQueue = Cast<SDirectXCommandQueue>(pQueue);
	ID3D12CommandQueue* lQueue = sQueue->pQueue.Get();

	ComPtr<IDXGISwapChain1> pSwapChain1;
	HR(pFactory->CreateSwapChainForHwnd(lQueue, (HWND)IApplicationInterface::Get().GetWindowHandle(), &SwapChainDesc, nullptr, nullptr, &pSwapChain1));

	ComPtr<IDXGISwapChain4> pSwapChain4;
	HR(pSwapChain1.As(&pSwapChain4));

	return gcnew SDirectXSwapChain(Cast<SDirectXDevice>(pQueue->GetDevice()), std::move(pSwapChain4), numBuffers);
}

void SDirectXFactory::Dispose(bool bDisposing)
{
	pFactory.Reset();
}