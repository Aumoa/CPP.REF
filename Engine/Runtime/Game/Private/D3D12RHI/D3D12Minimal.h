// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#undef interface

#include <dxgi1_6.h>
#include <d3d12.h>
#include <wrl/client.h>

#include "Logging/LogCategoryBase.h"
#include "HResultException.h"
#include "RHI/RHIResourceStates.h"

#ifndef interface
#define interface struct
#endif

namespace SC::Runtime::Game::D3D12RHI
{
	using Microsoft::WRL::ComPtr;

	extern Logging::LogCategoryBase LogD3D12RHI;
}

#define HR(x) if (FAILED(x)) throw SC::Runtime::Game::D3D12RHI::HResultException(x)

inline D3D12_RESOURCE_STATES ToD3D12(SC::Runtime::Game::RHI::RHIResourceStates value)
{
	using namespace SC::Runtime::Game::RHI;

	switch (value)
	{
	case RHIResourceStates::PRESENT:
		return D3D12_RESOURCE_STATE_PRESENT;
	case RHIResourceStates::RENDER_TARGET:
		return D3D12_RESOURCE_STATE_RENDER_TARGET;
	default:
		return D3D12_RESOURCE_STATE_COMMON;
	}
}

inline SC::Runtime::Game::RHI::RHIResourceStates ToRHI(D3D12_RESOURCE_STATES value)
{
	using namespace SC::Runtime::Game::RHI;

	switch (value)
	{
	case D3D12_RESOURCE_STATE_PRESENT:
		return RHIResourceStates::PRESENT;
	case D3D12_RESOURCE_STATE_RENDER_TARGET:
		return RHIResourceStates::RENDER_TARGET;
	default:
		return RHIResourceStates::PRESENT;
	}
}