// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Matrix4x4.h"

using namespace std;
using namespace DirectX;

inline XMMATRIX XMLoadMatrix4x4(const Matrix4x4* m)
{
	return XMLoadFloat4x4((const XMFLOAT4X4*)m);
}

inline XMVECTOR XMLoadVector3(const Vector3* v)
{
	return XMLoadFloat3((const XMFLOAT3*)v);
}

inline XMVECTOR XMLoadQuaternion(const Quaternion* v)
{
	return XMLoadFloat4((const XMFLOAT4*)v);
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

Vector3 Matrix4x4::TransformVector(const Vector3& vec) const
{
	XMMATRIX M = XMLoadMatrix4x4(this);
	XMVECTOR V = XMVector3Transform(XMVectorSet(vec.X(), vec.Y(), vec.Z(), 1.0f), M);
	Vector3 v;
	XMStoreVector3(v, V);
	return v;
}

Vector3 Matrix4x4::TransformNormal(const Vector3& vec) const
{
	XMMATRIX M = XMLoadMatrix4x4(this);
	XMVECTOR V = XMVector3TransformNormal(XMVectorSet(vec.X(), vec.Y(), vec.Z(), 0.0f), M);
	Vector3 v;
	XMStoreVector3(v, V);
	return v;
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

Matrix4x4 Matrix4x4::LookToLH(const Vector3& location, const Vector3& dir, const Vector3& up)
{
	XMMATRIX M = XMMatrixLookToLH(XMLoadVector3(&location), XMLoadVector3(&dir), XMLoadVector3(&up));
	return XMStoreMatrix4x4(M);
}

Matrix4x4 Matrix4x4::PerspectiveFovLH(Radians fovAngle, float aspectRatio, float zNearPlane, float zFarPlane)
{
	XMMATRIX M = XMMatrixPerspectiveFovLH(fovAngle.Value, aspectRatio, zNearPlane, zFarPlane);
	return XMStoreMatrix4x4(M);
}

Matrix4x4 Matrix4x4::RotationX(Radians angle)
{
	XMMATRIX M = XMMatrixRotationX(angle.Value);
	return XMStoreMatrix4x4(M);
}

Matrix4x4 Matrix4x4::RotationY(Radians angle)
{
	XMMATRIX M = XMMatrixRotationY(angle.Value);
	return XMStoreMatrix4x4(M);
}

Matrix4x4 Matrix4x4::RotationZ(Radians angle)
{
	XMMATRIX M = XMMatrixRotationZ(angle.Value);
	return XMStoreMatrix4x4(M);
}