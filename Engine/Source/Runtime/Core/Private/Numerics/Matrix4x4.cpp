// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Numerics/Matrix4x4.h"

#if PLATFORM_WINDOWS
#include "PlatformMisc/DirectXInlineHelper.h"

float Matrix4x4::GetDeterminant() const
{
	DirectX::XMMATRIX M = XMLoadMatrix4x4(this);
	return DirectX::XMVectorGetX(DirectX::XMMatrixDeterminant(M));
}

Matrix4x4 Matrix4x4::GetInverse() const
{
	DirectX::XMMATRIX M = XMLoadMatrix4x4(this);
	DirectX::XMVECTOR D = DirectX::XMMatrixDeterminant(M);
	DirectX::XMMATRIX I = DirectX::XMMatrixInverse(&D, M);

	return XMStoreMatrix4x4(I);
}

void Matrix4x4::Decompose(Vector3& outTranslation, Vector3& outScale, Quaternion& outRotation) const
{
	DirectX::XMMATRIX M = XMLoadMatrix4x4(this);
	DirectX::XMVECTOR T;
	DirectX::XMVECTOR S;
	DirectX::XMVECTOR Q;
	DirectX::XMMatrixDecompose(&S, &Q, &T, M);

	outTranslation = XMStoreVector3(T);
	outScale = XMStoreVector3(S);
	outRotation = XMStoreQuaternion(Q);
}

Vector3 Matrix4x4::TransformVector(const Vector3& vec) const
{
	DirectX::XMMATRIX M = XMLoadMatrix4x4(this);
	DirectX::XMVECTOR V = DirectX::XMVector3Transform(DirectX::XMVectorSet(vec.X, vec.Y, vec.Z, 1.0f), M);
	return XMStoreVector3(V);
}

Vector3 Matrix4x4::TransformNormal(const Vector3& vec) const
{
	DirectX::XMMATRIX M = XMLoadMatrix4x4(this);
	DirectX::XMVECTOR V = DirectX::XMVector3TransformNormal(DirectX::XMVectorSet(vec.X, vec.Y, vec.Z, 0.0f), M);
	return XMStoreVector3(V);
}

Matrix4x4 Matrix4x4::Multiply(const Matrix4x4& lhs, const Matrix4x4& rhs)
{
	DirectX::XMMATRIX LHS = XMLoadMatrix4x4(&lhs);
	DirectX::XMMATRIX RHS = XMLoadMatrix4x4(&rhs);
	return XMStoreMatrix4x4(DirectX::XMMatrixMultiply(LHS, RHS));
}

Matrix4x4 Matrix4x4::AffineTransformation(const Vector3& t, const Vector3& s, const Quaternion& q)
{
	DirectX::XMMATRIX M = DirectX::XMMatrixAffineTransformation(XMLoadVector3(&s), DirectX::XMVectorZero(), XMLoadQuaternion(&q), XMLoadVector3(&t));
	return XMStoreMatrix4x4(M);
}

Matrix4x4 Matrix4x4::LookToLH(const Vector3& location, const Vector3& dir, const Vector3& up)
{
	DirectX::XMMATRIX M = DirectX::XMMatrixLookToLH(XMLoadVector3(&location), XMLoadVector3(&dir), XMLoadVector3(&up));
	return XMStoreMatrix4x4(M);
}

Matrix4x4 Matrix4x4::PerspectiveFovLH(Radians fovAngle, float aspectRatio, float zNearPlane, float zFarPlane)
{
	DirectX::XMMATRIX M = DirectX::XMMatrixPerspectiveFovLH(fovAngle.Value, aspectRatio, zNearPlane, zFarPlane);
	return XMStoreMatrix4x4(M);
}

Matrix4x4 Matrix4x4::RotationX(Radians angle)
{
	DirectX::XMMATRIX M = DirectX::XMMatrixRotationX(angle.Value);
	return XMStoreMatrix4x4(M);
}

Matrix4x4 Matrix4x4::RotationY(Radians angle)
{
	DirectX::XMMATRIX M = DirectX::XMMatrixRotationY(angle.Value);
	return XMStoreMatrix4x4(M);
}

Matrix4x4 Matrix4x4::RotationZ(Radians angle)
{
	DirectX::XMMATRIX M = DirectX::XMMatrixRotationZ(angle.Value);
	return XMStoreMatrix4x4(M);
}

#endif