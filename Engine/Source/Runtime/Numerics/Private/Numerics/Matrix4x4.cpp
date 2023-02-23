// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Numerics/MatrixInterface/Matrix4x4.h"

#if PLATFORM_WINDOWS
#include "DirectXInlineHelper.h"

float Matrix4x4::Determinant(const Matrix4x4& M)
{
	DirectX::XMMATRIX XM = XMLoadMatrix4x4(&M);
	return DirectX::XMVectorGetX(DirectX::XMMatrixDeterminant(XM));
}

Matrix4x4 Matrix4x4::Inverse(const Matrix4x4& M)
{
	DirectX::XMMATRIX XM = XMLoadMatrix4x4(&M);
	DirectX::XMVECTOR XD = DirectX::XMMatrixDeterminant(XM);
	DirectX::XMMATRIX XI = DirectX::XMMatrixInverse(&XD, XM);

	return XMStoreMatrix4x4(XI);
}

bool Matrix4x4::Decompose(const Matrix4x4& M, Translate3D& OutT, Scale3D& OutS, Quaternion& OutQ)
{
	DirectX::XMMATRIX XM = XMLoadMatrix4x4(&M);
	DirectX::XMVECTOR XT;
	DirectX::XMVECTOR XS;
	DirectX::XMVECTOR XQ;
	bool b = DirectX::XMMatrixDecompose(&XS, &XQ, &XT, XM);
	if (b)
	{
		OutT = XMStoreVector3(XT);
		OutS = XMStoreVector3(XS);
		OutQ = XMStoreQuaternion(XQ);
	}
	return b;
}


Vector3 Matrix4x4::TransformPoint(const Matrix4x4& M, const Vector3& P)
{
	DirectX::XMMATRIX XM = XMLoadMatrix4x4(&M);
	DirectX::XMVECTOR XV = DirectX::XMVector3Transform(DirectX::XMVectorSet(P.X, P.Y, P.Z, 1.0f), XM);
	return XMStoreVector3(XV);
}

Vector3 Matrix4x4::TransformVector(const Matrix4x4& M, const Vector3& V)
{
	DirectX::XMMATRIX XM = XMLoadMatrix4x4(&M);
	DirectX::XMVECTOR XV = DirectX::XMVector3TransformNormal(DirectX::XMVectorSet(V.X, V.Y, V.Z, 0.0f), XM);
	return XMStoreVector3(XV);
}

Matrix4x4 Matrix4x4::Multiply(const Matrix4x4& ML, const Matrix4x4& MR)
{
	DirectX::XMMATRIX LHS = XMLoadMatrix4x4(&ML);
	DirectX::XMMATRIX RHS = XMLoadMatrix4x4(&MR);
	return XMStoreMatrix4x4(DirectX::XMMatrixMultiply(LHS, RHS));
}


Matrix4x4 Matrix4x4::LookToLH(const Vector3& Location, const Vector3& Dir, const Vector3& Up)
{
	DirectX::XMMATRIX M = DirectX::XMMatrixLookToLH(XMLoadVector3(&Location), XMLoadVector3(&Dir), XMLoadVector3(&Up));
	return XMStoreMatrix4x4(M);
}

Matrix4x4 Matrix4x4::PerspectiveFovLH(Radians Fov, float AspectRatio, float Near, float Far)
{
	DirectX::XMMATRIX M = DirectX::XMMatrixPerspectiveFovLH(Fov.Value, AspectRatio, Near, Far);
	return XMStoreMatrix4x4(M);
}

Matrix4x4 Matrix4x4::AffineTransformation(const Translate3D& T, const Scale3D& S, const Quaternion& Q)
{
	DirectX::XMMATRIX M = DirectX::XMMatrixAffineTransformation(XMLoadVector3(&S), DirectX::XMVectorZero(), XMLoadQuaternion(&Q), XMLoadVector3(&T));
	return XMStoreMatrix4x4(M);
}

Matrix4x4 Matrix4x4::Translation(const Vector3& T)
{
	DirectX::XMMATRIX M = DirectX::XMMatrixTranslation(T.X, T.Y, T.Z);
	return XMStoreMatrix4x4(M);
}

Matrix4x4 Matrix4x4::Scale(const Scale3D& S)
{
	DirectX::XMMATRIX M = DirectX::XMMatrixScaling(S.X, S.Y, S.Z);
	return XMStoreMatrix4x4(M);
}

Matrix4x4 Matrix4x4::Rotation(const Vector3& Axis, Radians Angle)
{
	DirectX::XMMATRIX M = DirectX::XMMatrixRotationAxis(DirectX::XMVectorSet(Axis.X, Axis.Y, Axis.Z, 0), Angle.Value);
	return XMStoreMatrix4x4(M);
}

Matrix4x4 Matrix4x4::RotationX(Radians Angle)
{
	DirectX::XMMATRIX M = DirectX::XMMatrixRotationX(Angle.Value);
	return XMStoreMatrix4x4(M);
}

Matrix4x4 Matrix4x4::RotationY(Radians Angle)
{
	DirectX::XMMATRIX M = DirectX::XMMatrixRotationY(Angle.Value);
	return XMStoreMatrix4x4(M);
}

Matrix4x4 Matrix4x4::RotationZ(Radians Angle)
{
	DirectX::XMMATRIX M = DirectX::XMMatrixRotationZ(Angle.Value);
	return XMStoreMatrix4x4(M);
}

#endif