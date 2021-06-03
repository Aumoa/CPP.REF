// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include <Windows.h>
#include <dxgi1_6.h>
#include <d3d12.h>
#include <comdef.h>

export module SC.Runtime.RenderCore.Internal;

import SC.Runtime.Core;

export
{
	/* ----------- Type declaration */
	typedef HRESULT HRESULT;
	typedef IID IID;
	typedef COINIT COINIT;
	typedef DXGI_ADAPTER_FLAG DXGI_ADAPTER_FLAG;

	/* ----------- class declaration */
	class _com_error;

	/* ----------- Interface declaration */
	interface IUnknown;
	interface IDXGIFactory2;
	interface IDXGISwapChain;
	interface ID3D12Debug;
	interface ID3D12Device;

	/* ----------- Function declaration */
	WINOLEAPI CoInitializeEx(LPVOID pvReserved, DWORD dwCoInit);
	HRESULT WINAPI CreateDXGIFactory2(UINT Flags, REFIID riid, void** ppFactory);
	HRESULT WINAPI D3D12GetDebugInterface(REFIID riid, void** ppvDebug);
	HRESULT WINAPI D3D12CreateDevice(IUnknown* pAdapter, D3D_FEATURE_LEVEL MinimumFeatureLevel, REFIID riid, void** ppDevice);

	/* ----------- Macro declaration */
#undef FAILED
	inline constexpr bool FAILED(HRESULT hr) { return hr < 0; }
#undef SUCCEEDED
	inline constexpr bool SUCCEEDED(HRESULT hr) { return hr >= 0; }
#undef DXGI_CREATE_FACTORY_DEBUG
	constexpr uint32 DXGI_CREATE_FACTORY_DEBUG = 0x1;
}