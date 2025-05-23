// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#if PLATFORM_WINDOWS

#include "Numerics/MatrixInterface/Matrix4x4.h"
#include <DirectXMath.h>

namespace Ayla
{
	inline ::DirectX::XMVECTOR XMLoadQuaternion(const void* ptr)
	{
		auto* float4 = (::DirectX::XMFLOAT4*)ptr;
		return ::DirectX::XMLoadFloat4(float4);
	}

	inline ::DirectX::XMVECTOR XMLoadVector3(const void* v)
	{
		auto* float3 = (const ::DirectX::XMFLOAT3*)v;
		return ::DirectX::XMLoadFloat3(float3);
	}

	inline Vector3F XMStoreVector3(::DirectX::FXMVECTOR V)
	{
		Vector3F float3;
		::DirectX::XMStoreFloat3((::DirectX::XMFLOAT3*)&float3, V);
		return float3;
	}

	inline QuaternionF XMStoreQuaternion(::DirectX::FXMVECTOR V)
	{
		QuaternionF q;
		::DirectX::XMStoreFloat4((::DirectX::XMFLOAT4*)&q, V);
		return q;
	}

	inline ::DirectX::XMMATRIX XMLoadMatrix4x4(const Matrix4x4F* m)
	{
		return ::DirectX::XMLoadFloat4x4((const ::DirectX::XMFLOAT4X4*)m);
	}

	inline Matrix4x4F XMStoreMatrix4x4(::DirectX::FXMMATRIX M)
	{
		Matrix4x4F m;
		::DirectX::XMStoreFloat4x4((::DirectX::XMFLOAT4X4*)&m, M);
		return m;
	}
}

#endif