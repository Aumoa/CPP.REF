// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DirectX/DirectXSwapChain.h"

#include "DirectXCommon.h"
#include "DirectX/DirectXDeviceBundle.h"
#include "DirectX/DirectXCommandQueue.h"
#include "Windows/CoreWindow.h"

DirectXSwapChain::DirectXSwapChain(DirectXDeviceBundle* deviceBundle, DirectXCommandQueue* commandQueue, CoreWindow* target, DXGI_FORMAT format) : Super()
{
	IDXGIFactory2* factory = deviceBundle->GetFactory();

	DXGI_SWAP_CHAIN_DESC1 desc = { };
	desc.Format = format;
	desc.SampleDesc = { 1, 0 };
	desc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
	desc.BufferCount = _countof(buffers);
	desc.Scaling = DXGI_SCALING_STRETCH;
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	desc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

	TComPtr<IDXGISwapChain1> swapChain;
	HR(factory->CreateSwapChainForHwnd(commandQueue->Item, target->HWnd, &desc, nullptr, nullptr, &swapChain));
	this->swapChain = swapChain.As<IDXGISwapChain4>();
}

DirectXSwapChain::~DirectXSwapChain()
{

}

void DirectXSwapChain::Present()
{
	HR(swapChain->Present(0, 0));
}

void DirectXSwapChain::ResizeBuffers(int32 x, int32 y)
{
	buffers[0].Reset();
	buffers[1].Reset();
	buffers[2].Reset();

	HR(swapChain->ResizeBuffers(0, (UINT)x, (UINT)y, DXGI_FORMAT_UNKNOWN, 0));

	HR(swapChain->GetBuffer(0, IID_PPV_ARGS(&buffers[0])));
	SetNameAuto(buffers[0]);
	HR(swapChain->GetBuffer(1, IID_PPV_ARGS(&buffers[1])));
	SetNameAuto(buffers[1]);
	HR(swapChain->GetBuffer(2, IID_PPV_ARGS(&buffers[2])));
	SetNameAuto(buffers[2]);
}

ID3D12Resource* DirectXSwapChain::GetCurrentBuffer() const
{
	return buffers[swapChain->GetCurrentBackBufferIndex()].Get();
}