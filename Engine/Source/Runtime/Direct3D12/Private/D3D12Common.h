// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#pragma push_macro("TEXT")
#include <d3d12.h>
#include <dxgi1_5.h>
#include <d3d12sdklayers.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
#include <comdef.h>
#include "Platform/Windows/UndefWindowsPlatformMacros.h"
#pragma pop_macro("TEXT")
#include "LogD3D12.h"
#include "Rendering/GraphicsFormat.h"

using Microsoft::WRL::ComPtr;

inline void HR__impl(Ayla::String s)
{
	Ayla::LogD3D12::Critical(s);
	throw Ayla::InvalidOperationException(s);
}

inline Ayla::String HR__format(HRESULT value)
{
	static thread_local std::map<HRESULT, _com_error> sCache;
	auto it = sCache.find(value);
	if (it == sCache.end())
	{
		it = sCache.emplace(value, value).first;
	}
	const wchar_t* msg = it->second.ErrorMessage();
	return Ayla::String::FromLiteral(msg);
}

#define HR(expr, ...) \
if (auto HR_res__ = (expr); FAILED(HR_res__)) \
{ \
	HR__impl(HR__format(HR_res__)); \
}

#if DO_CHECK
inline void DXSetName_Implementation(auto* class_, auto ptr, const wchar_t* callerName)
{
	std::string_view className1 = typeid(*class_).name();
	std::wstring className2(className1.begin(), className1.end());
	std::wstring cmp = className2 + L"." + callerName;
	HR(ptr->SetName(cmp.c_str()));
}

#define DXSetName(ptr) DXSetName_Implementation(this, ptr, L ## #ptr)
#else
#define DXSetName(ptr) ((void)ptr)
#endif

inline DXGI_FORMAT ToDXGIFormat(Ayla::GraphicsFormat format)
{
	using enum Ayla::GraphicsFormat;
	switch (format)
	{
	case R32G32B32A32_Float:   return DXGI_FORMAT_R32G32B32A32_FLOAT;
	case R32G32B32A32_UInt:    return DXGI_FORMAT_R32G32B32A32_UINT;
	case R32G32B32A32_SInt:    return DXGI_FORMAT_R32G32B32A32_SINT;
	case R32G32B32_Float:      return DXGI_FORMAT_R32G32B32_FLOAT;
	case R32G32B32_UInt:       return DXGI_FORMAT_R32G32B32_UINT;
	case R32G32B32_SInt:       return DXGI_FORMAT_R32G32B32_SINT;
	case R16G16B16A16_Float:   return DXGI_FORMAT_R16G16B16A16_FLOAT;
	case R16G16B16A16_UNorm:   return DXGI_FORMAT_R16G16B16A16_UNORM;
	case R16G16B16A16_UInt:    return DXGI_FORMAT_R16G16B16A16_UINT;
	case R16G16B16A16_SNorm:   return DXGI_FORMAT_R16G16B16A16_SNORM;
	case R16G16B16A16_SInt:    return DXGI_FORMAT_R16G16B16A16_SINT;
	case R32G32_Float:         return DXGI_FORMAT_R32G32_FLOAT;
	case R32G32_UInt:          return DXGI_FORMAT_R32G32_UINT;
	case R32G32_SInt:          return DXGI_FORMAT_R32G32_SINT;
	case R10G10B10A2_UNorm:    return DXGI_FORMAT_R10G10B10A2_UNORM;
	case R10G10B10A2_UInt:     return DXGI_FORMAT_R10G10B10A2_UINT;
	case R11G11B10_Float:      return DXGI_FORMAT_R11G11B10_FLOAT;
	case R8G8B8A8_UNorm:       return DXGI_FORMAT_R8G8B8A8_UNORM;
	case R8G8B8A8_UNorm_SRGB:  return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	case R8G8B8A8_UInt:        return DXGI_FORMAT_R8G8B8A8_UINT;
	case R8G8B8A8_SNorm:       return DXGI_FORMAT_R8G8B8A8_SNORM;
	case R8G8B8A8_SInt:        return DXGI_FORMAT_R8G8B8A8_SINT;
	case R16G16_Float:         return DXGI_FORMAT_R16G16_FLOAT;
	case R16G16_UNorm:         return DXGI_FORMAT_R16G16_UNORM;
	case R16G16_UInt:          return DXGI_FORMAT_R16G16_UINT;
	case R16G16_SNorm:         return DXGI_FORMAT_R16G16_SNORM;
	case R16G16_SInt:          return DXGI_FORMAT_R16G16_SINT;
	case D32_Float:            return DXGI_FORMAT_D32_FLOAT;
	case R32_Float:            return DXGI_FORMAT_R32_FLOAT;
	case R32_UInt:             return DXGI_FORMAT_R32_UINT;
	case R32_SInt:             return DXGI_FORMAT_R32_SINT;
	case D24_UNorm_S8_UInt:    return DXGI_FORMAT_D24_UNORM_S8_UINT;
	case R8G8_UNorm:           return DXGI_FORMAT_R8G8_UNORM;
	case R8G8_UInt:            return DXGI_FORMAT_R8G8_UINT;
	case R8G8_SNorm:           return DXGI_FORMAT_R8G8_SNORM;
	case R8G8_SInt:            return DXGI_FORMAT_R8G8_SINT;
	case R16_Float:            return DXGI_FORMAT_R16_FLOAT;
	case D16_UNorm:            return DXGI_FORMAT_D16_UNORM;
	case R16_UNorm:            return DXGI_FORMAT_R16_UNORM;
	case R16_UInt:             return DXGI_FORMAT_R16_UINT;
	case R16_SNorm:            return DXGI_FORMAT_R16_SNORM;
	case R16_SInt:             return DXGI_FORMAT_R16_SINT;
	case R8_UNorm:             return DXGI_FORMAT_R8_UNORM;
	case R8_UInt:              return DXGI_FORMAT_R8_UINT;
	case R8_SNorm:             return DXGI_FORMAT_R8_SNORM;
	case R8_SInt:              return DXGI_FORMAT_R8_SINT;
	case A8_UNorm:             return DXGI_FORMAT_A8_UNORM;
	case BC1_UNorm:            return DXGI_FORMAT_BC1_UNORM;
	case BC1_UNorm_SRGB:       return DXGI_FORMAT_BC1_UNORM_SRGB;
	case BC2_UNorm:            return DXGI_FORMAT_BC2_UNORM;
	case BC2_UNorm_SRGB:       return DXGI_FORMAT_BC2_UNORM_SRGB;
	case BC3_UNorm:            return DXGI_FORMAT_BC3_UNORM;
	case BC3_UNorm_SRGB:       return DXGI_FORMAT_BC3_UNORM_SRGB;
	case BC4_UNorm:            return DXGI_FORMAT_BC4_UNORM;
	case BC4_SNorm:            return DXGI_FORMAT_BC4_SNORM;
	case BC5_UNorm:            return DXGI_FORMAT_BC5_UNORM;
	case BC5_SNorm:            return DXGI_FORMAT_BC5_SNORM;
	case BC6H_UF16:            return DXGI_FORMAT_BC6H_UF16;
	case BC6H_SF16:            return DXGI_FORMAT_BC6H_SF16;
	case BC7_UNorm:            return DXGI_FORMAT_BC7_UNORM;
	case BC7_UNorm_SRGB:       return DXGI_FORMAT_BC7_UNORM_SRGB;
	case B5G6R5_UNorm:         return DXGI_FORMAT_B5G6R5_UNORM;
	case B5G5R5A1_UNorm:       return DXGI_FORMAT_B5G5R5A1_UNORM;
	case B8G8R8A8_UNorm:       return DXGI_FORMAT_B8G8R8A8_UNORM;
	case B8G8R8A8_UNorm_SRGB:  return DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
	case B4G4R4A4_UNorm:       return DXGI_FORMAT_B4G4R4A4_UNORM;
	case A4B4G4R4_UNorm:       return DXGI_FORMAT_A4B4G4R4_UNORM;
	default:                   return DXGI_FORMAT_UNKNOWN;
	}
}