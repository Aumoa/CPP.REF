// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "pch.h"
#include "Numerics/Quaternion.h"
#include "Numerics/Matrix4x4.h"

inline DirectX::XMVECTOR XMLoadQuaternion(const Quaternion* ptr)
{
	auto* float4 = (DirectX::XMFLOAT4*)ptr;
	return DirectX::XMLoadFloat4(float4);
}

inline DirectX::XMVECTOR XMLoadVector3(const Vector3* v)
{
	auto* float3 = (const DirectX::XMFLOAT3*)v;
	return DirectX::XMLoadFloat3(float3);
}

inline Vector3 XMStoreVector3(DirectX::FXMVECTOR V)
{
	Vector3 float3;
	DirectX::XMStoreFloat3((DirectX::XMFLOAT3*)&float3, V);
	return float3;
}

inline Quaternion XMStoreQuaternion(DirectX::FXMVECTOR V)
{
	Quaternion q;
	DirectX::XMStoreFloat4((DirectX::XMFLOAT4*)&q, V);
	return q;
}

inline DirectX::XMMATRIX XMLoadMatrix4x4(const Matrix4x4* m)
{
	return DirectX::XMLoadFloat4x4((const DirectX::XMFLOAT4X4*)m);
}

inline Matrix4x4 XMStoreMatrix4x4(DirectX::FXMMATRIX M)
{
	Matrix4x4 m;
	DirectX::XMStoreFloat4x4((DirectX::XMFLOAT4X4*)&m, M);
	return m;
}