// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include <DirectXMath.h>

import std.core;
import SC.Runtime.Core;

using namespace std;
using namespace DirectX;

inline XMMATRIX XMLoadMatrix4x4(const Matrix4x4* m)
{
	return XMLoadFloat4x4((const XMFLOAT4X4*)&m);
}

inline XMVECTOR XMLoadVector3(const Vector3* v)
{
	return XMLoadFloat3((const XMFLOAT3*)&v);
}

inline XMVECTOR XMLoadQuaternion(const Quaternion* v)
{
	return XMLoadFloat4((const XMFLOAT4*)&v);
}

inline Matrix4x4 XMStoreMatrix4x4(FXMMATRIX M)
{
	Matrix4x4 m;
	XMStoreFloat4x4((XMFLOAT4X4*)&m, M);
	return m;
}

inline void XMStoreVector3(Vector3& v, FXMVECTOR V)
{
	XMStoreFloat3((XMFLOAT3*)&v, V);
}

inline void XMStoreQuaternion(Quaternion& q, XMVECTOR Q)
{
	XMStoreFloat4((XMFLOAT4*)&q, Q);
}

float Matrix4x4::GetDeterminant() const
{
	XMMATRIX M = XMLoadMatrix4x4(this);
	return XMVectorGetX(XMMatrixDeterminant(M));
}

Matrix4x4 Matrix4x4::GetInverse() const
{
	XMMATRIX M = XMLoadMatrix4x4(this);
	XMVECTOR D = XMMatrixDeterminant(M);
	XMMATRIX I = XMMatrixInverse(&D, M);

	return XMStoreMatrix4x4(I);
}

void Matrix4x4::Decompose(Vector3& outTranslation, Vector3& outScale, Quaternion& outRotation) const
{
	XMMATRIX M = XMLoadMatrix4x4(this);
	XMVECTOR T;
	XMVECTOR S;
	XMVECTOR Q;
	XMMatrixDecompose(&S, &Q, &T, M);

	XMStoreVector3(outTranslation, T);
	XMStoreVector3(outScale, S);
	XMStoreQuaternion(outRotation, Q);
}

Matrix4x4 Matrix4x4::Multiply(const Matrix4x4& lhs, const Matrix4x4& rhs)
{
	XMMATRIX LHS = XMLoadMatrix4x4(&lhs);
	XMMATRIX RHS = XMLoadMatrix4x4(&rhs);
	return XMStoreMatrix4x4(XMMatrixMultiply(LHS, RHS));
}

Matrix4x4 Matrix4x4::AffineTransformation(const Vector3& t, const Vector3& s, const Quaternion& q)
{
	XMMATRIX M = XMMatrixAffineTransformation(XMLoadVector3(&s), XMVectorZero(), XMLoadQuaternion(&q), XMLoadVector3(&t));
	return XMStoreMatrix4x4(M);
}