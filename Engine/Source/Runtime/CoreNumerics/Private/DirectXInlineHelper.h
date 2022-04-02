// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#if PLATFORM_WINDOWS

#include <DirectXMath.h>
#include "Numerics/MatrixInterface/Matrix4x4.h"
#include "Numerics/TransformInterface/Quaternion.h"

inline DirectX::XMVECTOR XMLoadQuaternion(const void* ptr)
{
	auto* float4 = (DirectX::XMFLOAT4*)ptr;
	return DirectX::XMLoadFloat4(float4);
}

inline DirectX::XMVECTOR XMLoadVector3(const void* v)
{
	auto* float3 = (const DirectX::XMFLOAT3*)v;
	return DirectX::XMLoadFloat3(float3);
}

inline libty::Vector3 XMStoreVector3(DirectX::FXMVECTOR V)
{
	libty::Vector3 float3;
	DirectX::XMStoreFloat3((DirectX::XMFLOAT3*)&float3, V);
	return float3;
}

inline libty::Quaternion XMStoreQuaternion(DirectX::FXMVECTOR V)
{
	libty::Quaternion q;
	DirectX::XMStoreFloat4((DirectX::XMFLOAT4*)&q, V);
	return q;
}

inline DirectX::XMMATRIX XMLoadMatrix4x4(const libty::Matrix4x4* m)
{
	return DirectX::XMLoadFloat4x4((const DirectX::XMFLOAT4X4*)m);
}

inline libty::Matrix4x4 XMStoreMatrix4x4(DirectX::FXMMATRIX M)
{
	libty::Matrix4x4 m;
	DirectX::XMStoreFloat4x4((DirectX::XMFLOAT4X4*)&m, M);
	return m;
}

#endif