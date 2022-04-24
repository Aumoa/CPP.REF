// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include <wrl/client.h>
#include <d3d12.h>
#include <d3d11.h>
#include <d3d11on12.h>
#include <dxgi1_6.h>
#include <d2d1_1.h>
#include <d3dcompiler.h>
#include <dwrite_3.h>
#undef Yield

DECLARE_LOG_CATEGORY(DIRECTX_API, LogDirectX);

#define DECLARE_GETTER(Type, Member)												\
	template<std::same_as<Type> T> requires requires { Member.Get(); }				\
	inline T* Get(short) { return static_cast<Type*>(Member.Get()); }				\
	template<std::same_as<Type> T> requires requires { static_cast<Type*>(Member); }\
	inline T* Get(int) { return static_cast<Type*>(Member); }						\
	template<std::same_as<Type> T>													\
	inline T* Get() { return Get<T>(0); }

namespace libty::inline DirectX
{
	inline std::string_view ShaderTypeToTarget(ERHIShaderType Type)
	{
		switch (Type)
		{
		case ERHIShaderType::VertexShader:
			return "vs_5_0";
		case ERHIShaderType::PixelShader:
			return "ps_5_0";
		case ERHIShaderType::DomainShader:
			return "ds_5_0";
		case ERHIShaderType::HullShader:
			return "hs_5_0";
		case ERHIShaderType::GeometryShader:
			return "gs_5_0";
		case ERHIShaderType::ComputeShader:
			return "cs_5_0";
		default:
			return "";
		}
	}

	inline UINT GetInteropBindFlag(D3D12_RESOURCE_FLAGS flags)
	{
		UINT bindFlags = D3D11_BIND_SHADER_RESOURCE;
		if (flags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET)
		{
			bindFlags |= D3D11_BIND_RENDER_TARGET;
		}
		return bindFlags;
	}

	using Microsoft::WRL::ComPtr;

	void ReplaceNativePointer(std::vector<D3D12_RESOURCE_BARRIER>& dst, std::span<const RHIResourceBarrier> src);
	void ReplaceNativePointer(std::vector<ID3D12CommandList*>& dst, std::span<IRHIGraphicsCommandList* const> src);
}