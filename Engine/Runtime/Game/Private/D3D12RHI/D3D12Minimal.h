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

namespace SC::Runtime::Game::D3D12RHI
{
	using Microsoft::WRL::ComPtr;

	extern Logging::LogCategoryBase LogD3D12RHI;
}

#define HR(x) if (FAILED(x)) throw SC::Runtime::Game::D3D12RHI::HResultException(x)

D3D12_RESOURCE_STATES ToD3D12(SC::Runtime::Game::RHI::RHIResourceStates value);
DXGI_FORMAT ToD3D12(SC::Runtime::Game::RHI::RHITextureFormat value);
D3D12_RESOURCE_FLAGS ToD3D12(SC::Runtime::Game::RHI::RHIResourceFlags value);

SC::Runtime::Game::RHI::RHIResourceStates ToRHI(D3D12_RESOURCE_STATES value);
SC::Runtime::Game::RHI::RHITextureFormat ToRHI(DXGI_FORMAT value);
SC::Runtime::Game::RHI::RHIResourceFlags ToRHI(D3D12_RESOURCE_FLAGS value);

bool IsDepthStencilFormat(SC::Runtime::Game::RHI::RHITextureFormat value);