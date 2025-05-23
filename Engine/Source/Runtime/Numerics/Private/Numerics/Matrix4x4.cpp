// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Numerics/MatrixInterface/Matrix4x4.h"

#if PLATFORM_WINDOWS
#include "DirectXInlineHelper.h"

namespace Ayla
{
	float SIMDMatrix::Determinant(const Matrix4x4<float>& M)
	{
		DirectX::XMMATRIX XM = XMLoadMatrix4x4(&M);
		return DirectX::XMVectorGetX(DirectX::XMMatrixDeterminant(XM));
	}

	Matrix4x4<float> SIMDMatrix::Inverse(const Matrix4x4<float>& M)
	{
		DirectX::XMMATRIX XM = XMLoadMatrix4x4(&M);
		DirectX::XMVECTOR XD = DirectX::XMMatrixDeterminant(XM);
		DirectX::XMMATRIX XI = DirectX::XMMatrixInverse(&XD, XM);

		return XMStoreMatrix4x4(XI);
	}

	bool SIMDMatrix::Decompose(const Matrix4x4<float>& M, Translate3DF& OutT, Scale3DF& OutS, QuaternionF& OutQ)
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


	Vector3F SIMDMatrix::TransformPoint(const Matrix4x4<float>& M, const Vector3F& P)
	{
		DirectX::XMMATRIX XM = XMLoadMatrix4x4(&M);
		DirectX::XMVECTOR XV = DirectX::XMVector3Transform(DirectX::XMVectorSet(P.X, P.Y, P.Z, 1.0f), XM);
		return XMStoreVector3(XV);
	}

	Vector3F SIMDMatrix::TransformVector(const Matrix4x4<float>& M, const Vector3F& V)
	{
		DirectX::XMMATRIX XM = XMLoadMatrix4x4(&M);
		DirectX::XMVECTOR XV = DirectX::XMVector3TransformNormal(DirectX::XMVectorSet(V.X, V.Y, V.Z, 0.0f), XM);
		return XMStoreVector3(XV);
	}

	Matrix4x4<float> SIMDMatrix::Multiply(const Matrix4x4<float>& ML, const Matrix4x4<float>& MR)
	{
		DirectX::XMMATRIX LHS = XMLoadMatrix4x4(&ML);
		DirectX::XMMATRIX RHS = XMLoadMatrix4x4(&MR);
		return XMStoreMatrix4x4(DirectX::XMMatrixMultiply(LHS, RHS));
	}


	Matrix4x4<float> SIMDMatrix::LookToLH(const Vector3F& Location, const Vector3F& Dir, const Vector3F& Up)
	{
		DirectX::XMMATRIX M = DirectX::XMMatrixLookToLH(XMLoadVector3(&Location), XMLoadVector3(&Dir), XMLoadVector3(&Up));
		return XMStoreMatrix4x4(M);
	}

	Matrix4x4<float> SIMDMatrix::PerspectiveFovLH(RadiansF Fov, float AspectRatio, float Near, float Far)
	{
		DirectX::XMMATRIX M = DirectX::XMMatrixPerspectiveFovLH(Fov.Value, AspectRatio, Near, Far);
		return XMStoreMatrix4x4(M);
	}

	Matrix4x4<float> SIMDMatrix::AffineTransformation(const Translate3DF& T, const Scale3DF& S, const QuaternionF& Q)
	{
		DirectX::XMMATRIX M = DirectX::XMMatrixAffineTransformation(XMLoadVector3(&S), DirectX::XMVectorZero(), XMLoadQuaternion(&Q), XMLoadVector3(&T));
		return XMStoreMatrix4x4(M);
	}

	Matrix4x4<float> SIMDMatrix::Translation(const Vector3F& T)
	{
		DirectX::XMMATRIX M = DirectX::XMMatrixTranslation(T.X, T.Y, T.Z);
		return XMStoreMatrix4x4(M);
	}

	Matrix4x4<float> SIMDMatrix::Scale(const Scale3DF& S)
	{
		DirectX::XMMATRIX M = DirectX::XMMatrixScaling(S.X, S.Y, S.Z);
		return XMStoreMatrix4x4(M);
	}

	Matrix4x4<float> SIMDMatrix::Rotation(const Vector3F& Axis, RadiansF Angle)
	{
		DirectX::XMMATRIX M = DirectX::XMMatrixRotationAxis(DirectX::XMVectorSet(Axis.X, Axis.Y, Axis.Z, 0), Angle.Value);
		return XMStoreMatrix4x4(M);
	}

	Matrix4x4<float> SIMDMatrix::RotationX(RadiansF Angle)
	{
		DirectX::XMMATRIX M = DirectX::XMMatrixRotationX(Angle.Value);
		return XMStoreMatrix4x4(M);
	}

	Matrix4x4<float> SIMDMatrix::RotationY(RadiansF Angle)
	{
		DirectX::XMMATRIX M = DirectX::XMMatrixRotationY(Angle.Value);
		return XMStoreMatrix4x4(M);
	}

	Matrix4x4<float> SIMDMatrix::RotationZ(RadiansF Angle)
	{
		DirectX::XMMATRIX M = DirectX::XMMatrixRotationZ(Angle.Value);
		return XMStoreMatrix4x4(M);
	}
}

#endif