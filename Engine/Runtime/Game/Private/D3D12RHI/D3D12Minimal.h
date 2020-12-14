// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#undef interface

#include <dxgi1_6.h>
#include <d3d12.h>
#include <wrl/client.h>

#include "Logging/LogCategoryBase.h"
#include "HResultException.h"

#ifndef interface
#define interface struct
#endif

namespace SC::Runtime::Game::D3D12RHI
{
	using Microsoft::WRL::ComPtr;

	extern Logging::LogCategoryBase LogD3D12RHI;
}

#define HR(x) if (FAILED(x)) throw SC::Runtime::Game::D3D12RHI::HResultException(x)