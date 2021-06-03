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

	/* ----------- class declaration */
	class _com_error;

	/* ----------- Interface declaration */
	interface IUnknown;
	interface IDXGIFactory2;
	interface IDXGISwapChain;
	interface IDXGISwapChain4;
	interface ID3D12Debug;
	interface ID3D12Device;
	interface ID3D12CommandQueue;

	/* ----------- Macro declaration */
#undef FAILED
	inline constexpr bool FAILED(HRESULT hr) { return hr < 0; }
#undef SUCCEEDED
	inline constexpr bool SUCCEEDED(HRESULT hr) { return hr >= 0; }
}