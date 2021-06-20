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

Quaternion Quaternion::LookTo(const Vector3& forward, const Vector3& up)
{
	Quaternion t;

	Vector3 F = forward.GetNormal();
	Vector3 U = up.GetNormal();
	Vector3 R = Vector3::CrossProduct(up, forward).GetNormal();
	U = Vector3::CrossProduct(F, R);

	XMMATRIX M = XMMatrixSet(
		R.X(), R.Y(), R.Z(), 0,
		U.X(), U.Y(), U.Z(), 0,
		F.X(), F.Y(), F.Z(), 0,
		0, 0, 0, 1.0f
	);

	XMVECTOR Q = XMQuaternionRotationMatrix(M);
	return XMStoreQuaternion(Q);
}