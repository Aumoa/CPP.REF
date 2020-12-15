// Copyright 2020 Aumoa.lib. All right reserved.

#include "D3D12SwapChain.h"

using namespace SC::Runtime::Game::D3D12RHI;

D3D12SwapChain::D3D12SwapChain(IDXGISwapChain4* swapChain) : Super()
{
	this->swapChain = swapChain;
}

D3D12SwapChain::~D3D12SwapChain()
{

}

bool D3D12SwapChain::Present()
{
	HRESULT hr = swapChain->Present(0, 0);
	if (hr == DXGI_STATUS_OCCLUDED)
	{
		return false;
	}

	if (FAILED(hr))
	{
		throw HResultException(hr);
	}

	return true;
}

bool D3D12SwapChain::PresentTest()
{
	HRESULT hr = swapChain->Present(0, DXGI_PRESENT_TEST);
	if (hr == DXGI_STATUS_OCCLUDED)
	{
		return false;
	}

	if (FAILED(hr))
	{
		throw HResultException(hr);
	}

	return true;
}

int32 D3D12SwapChain::CurrentBackBufferIndex_get() const
{
	return (int32)swapChain->GetCurrentBackBufferIndex();
}