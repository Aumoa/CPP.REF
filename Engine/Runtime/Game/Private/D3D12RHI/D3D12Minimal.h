// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#undef interface

#include <dxgi1_6.h>
#include <d3d12.h>
#include <wrl/client.h>

#include "Logging/LogCategoryBase.h"
#include "HResultException.h"
#include "RHI/RHIResourceStates.h"
#include "RHI/RHITextureFormat.h"
#include "RHI/RHIResourceFlags.h"

#ifndef interface
#define interface struct
#endif

using Microsoft::WRL::ComPtr;

extern LogCategoryBase LogD3D12RHI;

#ifdef _DEBUG
#define HR(x) if (HRESULT hr = x; FAILED(hr)) throw HResultException(hr)
#else
#define HR(x) x
#endif

struct CD3DX12_CPU_DESCRIPTOR_HANDLE : public D3D12_CPU_DESCRIPTOR_HANDLE
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE();
	CD3DX12_CPU_DESCRIPTOR_HANDLE(const D3D12_CPU_DESCRIPTOR_HANDLE& handle);
	CD3DX12_CPU_DESCRIPTOR_HANDLE(const CD3DX12_CPU_DESCRIPTOR_HANDLE& handle);
	CD3DX12_CPU_DESCRIPTOR_HANDLE IncrementPost(uint32 increment);
};

struct CD3DX12_RESOURCE_BARRIER : public D3D12_RESOURCE_BARRIER
{
	CD3DX12_RESOURCE_BARRIER();

	CD3DX12_RESOURCE_BARRIER& SwapTransition();
	
	static CD3DX12_RESOURCE_BARRIER TransitionBarrier(ID3D12Resource* inResource, D3D12_RESOURCE_STATES inBefore, D3D12_RESOURCE_STATES inAfter, uint32 inSubresourceIdx = 0);
};

#define TRANSITION(x, before, after) CD3DX12_RESOURCE_BARRIER::TransitionBarrier((x).Get(), D3D12_RESOURCE_STATE_ ## before, D3D12_RESOURCE_STATE_ ## after)