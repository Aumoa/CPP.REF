// Copyright 2020 Aumoa.lib. All right reserved.

#include "D3D12DeviceBundle.h"

#include "Logging/Logger.h"
#include "Logging/LogMacros.h"
#include "Application.h"
#include "D3D12SwapChain.h"
#include "D3D12ImmediateCommandList.h"
#include "D3D12CommandFence.h"
#include "D3D12RenderTargetView.h"
#include "D3D12OfflineDescriptorManager.h"
#include "D3D12OfflineDescriptorIndex.h"
#include "D3D12Resource.h"

using namespace SC::Runtime::Core;
using namespace SC::Runtime::Game;
using namespace SC::Runtime::Game::RHI;
using namespace SC::Runtime::Game::D3D12RHI;

D3D12DeviceBundle* D3D12DeviceBundle::instance = nullptr;

D3D12DeviceBundle::D3D12DeviceBundle() : Super()
{
#ifdef _DEBUG
	if (ComPtr<ID3D12Debug> debug; SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug))))
	{
		debug->EnableDebugLayer();
		SE_LOG(LogD3D12RHI, Verbose, "D3D12 debug layer enabled.");
	}
	else
	{
		SE_LOG(LogD3D12RHI, Warning, "Cannot enable D3D12 debug layer.");
	}
#endif
}

D3D12DeviceBundle::~D3D12DeviceBundle()
{
	CoUninitialize();
}

void D3D12DeviceBundle::InitializeBundle()
{
	if (instance != nullptr)
	{
		SE_LOG(LogD3D12RHI, Error, "Bundle duplicate detected.");
		throw HResultException(E_FAIL);
	}

	SE_LOG(LogD3D12RHI, Verbose, "Initialize D3D12 device bundle.");

	InitializeCOM();
	InitializeDXGI();
	InitializeD3D12();

	instance = this;

	GApplication.Sizing += bind_delegate(Application_OnSizing);
}

void D3D12DeviceBundle::ReleaseBundle()
{
	instance = nullptr;
}

TRefPtr<IRHISwapChain> D3D12DeviceBundle::GetSwapChain() const
{
	return swapChain;
}

TRefPtr<IRHIImmediateCommandList> D3D12DeviceBundle::GetImmediateCommandList() const
{
	return immediateCommandList;
}

TRefPtr<IRHICommandFence> D3D12DeviceBundle::CreateCommandFence()
{
	return NewObject<D3D12CommandFence>(d3d12Device.Get());
}

TRefPtr<IRHIRenderTargetView> D3D12DeviceBundle::CreateRenderTargetView(IRHIResource* resource)
{
	ID3D12Resource* resource1 = Cast<D3D12Resource>(resource)->Resource;
	D3D12OfflineDescriptorIndex index = rtvManager->Alloc();
	d3d12Device->CreateRenderTargetView(resource1, nullptr, index.Handle);
	return NewObject<D3D12RenderTargetView>(resource1, index);
}

ID3D12Device* D3D12DeviceBundle::Device_get() const
{
	return d3d12Device.Get();
}

void D3D12DeviceBundle::InitializeCOM()
{
	HR(CoInitializeEx(nullptr, COINIT_MULTITHREADED));
}

void D3D12DeviceBundle::InitializeDXGI()
{
	UINT flag = 0;
#ifdef _DEBUG
	flag |= DXGI_CREATE_FACTORY_DEBUG;
#endif

	HR(CreateDXGIFactory2(flag, IID_PPV_ARGS(&dxgiFactory)));
}

void D3D12DeviceBundle::InitializeD3D12()
{
	constexpr size_t OfflineDescriptorAllocUnit = 128;

	ComPtr<IDXGIAdapter1> adapter;
	for (int32 i = 0; SUCCEEDED(dxgiFactory->EnumAdapters1((UINT)i, &adapter)); ++i)
	{
		if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&d3d12Device))))
		{
			TRefPtr<String> deviceName = L"Unknown";
			DXGI_ADAPTER_DESC1 desc1;
			if (SUCCEEDED(adapter->GetDesc1(&desc1)))
			{
				deviceName = desc1.Description;
			}

			SE_LOG(LogD3D12RHI, Verbose, "Succeeded to detect adapter that support {2} with index: {0}, name: {1}.", i, deviceName, nameof(D3D_FEATURE_LEVEL_12_1));
			break;
		}
	}

	if (!d3d12Device)
	{
		SE_LOG(LogD3D12RHI, Error, "Failed to detect adapter that support feature level 12_0. Create device with software platform.");
		throw HResultException(E_NOTIMPL);
	}

	// Create core command queue.
	immediateCommandList = NewObject<D3D12ImmediateCommandList>(d3d12Device.Get());

	DXGI_SWAP_CHAIN_DESC1 chainDesc{ };
	chainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	chainDesc.SampleDesc = { 1, 0 };
	chainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
	chainDesc.BufferCount = 3;
	chainDesc.Scaling = DXGI_SCALING_STRETCH;
	chainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	chainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

	HWND hWnd = Application::GetInstance()->GetCoreHwnd();
	ComPtr<IDXGISwapChain1> swapChain;
	HR(dxgiFactory->CreateSwapChainForHwnd(immediateCommandList->CommandQueue, hWnd, &chainDesc, nullptr, nullptr, &swapChain));
	ComPtr<IDXGISwapChain4> swapChain4;
	HR(swapChain.As(&swapChain4));

	this->swapChain = NewObject<D3D12SwapChain>(swapChain4.Get());

	rtvManager = NewObject<D3D12OfflineDescriptorManager>(d3d12Device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_RTV, OfflineDescriptorAllocUnit);
	dsvManager = NewObject<D3D12OfflineDescriptorManager>(d3d12Device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_RTV, OfflineDescriptorAllocUnit);
}

void D3D12DeviceBundle::Application_OnSizing(int32 width, int32 height)
{
	SE_LOG(LogD3D12RHI, Verbose, L"Swap chain resized to {0} x {1}", width, height);
	swapChain->ResizeBuffers(width, height);
}