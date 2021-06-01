// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include <DirectXMath.h>

import std.core;
import SC.Runtime.Core;

using namespace DirectX;

inline XMVECTOR ToVector(const Quaternion* ptr)
{
	auto* float4 = (XMFLOAT4*)ptr;
	return XMLoadFloat4(float4);
}

///
inline XMVECTOR ToVector(const Vector3* v)
{
	auto* float3 = (const XMFLOAT3*)&v;
	return XMLoadFloat3(float3);
}

inline Vector3 ToVector3(FXMVECTOR V)
{
	Vector3 float3;
	XMStoreFloat3((XMFLOAT3*)&float3, V);
	return float3;
}