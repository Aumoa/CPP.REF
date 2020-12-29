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

D3D12_RESOURCE_STATES ToD3D12(RHIResourceStates value);
DXGI_FORMAT ToD3D12(RHITextureFormat value);
D3D12_RESOURCE_FLAGS ToD3D12(RHIResourceFlags value);

RHIResourceStates ToRHI(D3D12_RESOURCE_STATES value);
RHITextureFormat ToRHI(DXGI_FORMAT value);
RHIResourceFlags ToRHI(D3D12_RESOURCE_FLAGS value);

bool IsDepthStencilFormat(RHITextureFormat value);