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

bool IsDepthStencilFormat(RHITextureFormat value);