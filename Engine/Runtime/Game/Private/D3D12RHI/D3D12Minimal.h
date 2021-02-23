// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#pragma warning(push)
#pragma warning(disable: 4005)
#include <dxgi1_6.h>
#include <d3d12.h>
#include <wrl/client.h>
#pragma warning(pop)

#include "Logging/LogCategoryBase.h"
#include "HResultException.h"
#include "RHI/RHIResourceStates.h"
#include "RHI/RHITextureFormat.h"
#include "RHI/RHIResourceFlags.h"

using Microsoft::WRL::ComPtr;

extern LogCategoryBase LogD3D12RHI;

#undef min
#undef max

#ifdef _DEBUG
#define HR(x) if (HRESULT hr = x; FAILED(hr)) throw HResultException(hr)
#else
#define HR(x) x
#endif

template<class T>
struct CD3DX12_ANY_DESCRIPTOR_HANDLE : public T
{
	CD3DX12_ANY_DESCRIPTOR_HANDLE()
	{

	}

	CD3DX12_ANY_DESCRIPTOR_HANDLE(const T& handle) : T(handle)
	{

	}

	CD3DX12_ANY_DESCRIPTOR_HANDLE(const CD3DX12_ANY_DESCRIPTOR_HANDLE& handle) : T(handle)
	{

	}

	CD3DX12_ANY_DESCRIPTOR_HANDLE& IncrementPre(uint32 increment, size_t count = 1)
	{
		this->T::ptr += (ssize_t)increment * count;
		return *this;
	}

	CD3DX12_ANY_DESCRIPTOR_HANDLE IncrementPost(uint32 increment, size_t count = 1)
	{
		auto left = *this;
		this->T::ptr += (ssize_t)increment * count;
		return left;
	}

	CD3DX12_ANY_DESCRIPTOR_HANDLE Get(uint32 increment, size_t count) const
	{
		auto left = *this;
		left.T::ptr += (size_t)increment * count;
		return left;
	}
};

using CD3DX12_CPU_DESCRIPTOR_HANDLE = CD3DX12_ANY_DESCRIPTOR_HANDLE<D3D12_CPU_DESCRIPTOR_HANDLE>;
using CD3DX12_GPU_DESCRIPTOR_HANDLE = CD3DX12_ANY_DESCRIPTOR_HANDLE<D3D12_GPU_DESCRIPTOR_HANDLE>;

struct CD3DX12_RESOURCE_BARRIER : public D3D12_RESOURCE_BARRIER
{
	CD3DX12_RESOURCE_BARRIER();

	CD3DX12_RESOURCE_BARRIER& SwapTransition();
	
	static CD3DX12_RESOURCE_BARRIER TransitionBarrier(ID3D12Resource* inResource, D3D12_RESOURCE_STATES inBefore, D3D12_RESOURCE_STATES inAfter, uint32 inSubresourceIdx = 0);
};

#define TRANSITION(x, before, after) CD3DX12_RESOURCE_BARRIER::TransitionBarrier((x)->Resource, D3D12_RESOURCE_STATE_ ## before, D3D12_RESOURCE_STATE_ ## after)