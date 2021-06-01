// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include <DirectXMath.h>

import std.core;
import SC.Runtime.Core;

using namespace std;
using namespace DirectX;

#define reinterpret (const XMFLOAT4X4*)V

float Matrix4x4::GetDeterminant() const
{
	XMMATRIX M = XMLoadFloat4x4(reinterpret);
	return XMVectorGetX(XMMatrixDeterminant(M));
}

Matrix4x4 Matrix4x4::GetInverse() const
{
	XMMATRIX M = XMLoadFloat4x4(reinterpret);
	XMVECTOR D = XMMatrixDeterminant(M);
	XMMATRIX I = XMMatrixInverse(&D, M);

	Matrix4x4 V;
	XMStoreFloat4x4((XMFLOAT4X4*)&V, I);
	return V;
}