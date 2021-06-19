// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Quaternion.h"
#include <DirectXMath.h>

using namespace DirectX;

inline XMVECTOR XMLoadQuaternion(const Quaternion* ptr)
{
	auto* float4 = (XMFLOAT4*)ptr;
	return XMLoadFloat4(float4);
}

inline XMVECTOR XMLoadVector3(const Vector3* v)
{
	auto* float3 = (const XMFLOAT3*)v;
	return XMLoadFloat3(float3);
}

inline Vector3 XMStoreVector3(FXMVECTOR V)
{
	Vector3 float3;
	XMStoreFloat3((XMFLOAT3*)&float3, V);
	return float3;
}

inline Quaternion XMStoreQuaternion(FXMVECTOR V)
{
	Quaternion q;
	XMStoreFloat4((XMFLOAT4*)&q, V);
	return q;
}

Quaternion Quaternion::FromAxisAngle(const Vector3& axis, Degrees angle)
{
	Radians rad = angle.ToRadians();
	XMVECTOR Axis = XMLoadVector3(&axis);
	XMVECTOR Q = XMQuaternionRotationNormal(Axis, rad.Value);
	return XMStoreQuaternion(Q);
}