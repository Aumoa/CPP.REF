// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Numerics/Quaternion.h"
#include "DirectXInlineHelper.h"

Quaternion Quaternion::FromAxisAngle(const Vector3& axis, Degrees angle)
{
	Radians rad = angle.ToRadians();
	DirectX::XMVECTOR Axis = XMLoadVector3(&axis);
	DirectX::XMVECTOR Q = DirectX::XMQuaternionRotationNormal(Axis, rad.Value);
	return XMStoreQuaternion(Q);
}

Quaternion Quaternion::LookTo(const Vector3& forward, const Vector3& up)
{
	Quaternion t;

	Vector3 F = forward.GetNormal();
	Vector3 U = up.GetNormal();
	Vector3 R = Vector3::CrossProduct(up, forward).GetNormal();
	U = Vector3::CrossProduct(F, R);

	DirectX::XMMATRIX M = DirectX::XMMatrixSet(
		R.X, R.Y, R.Z, 0,
		U.X, U.Y, U.Z, 0,
		F.X, F.Y, F.Z, 0,
		0, 0, 0, 1.0f
	);

	DirectX::XMVECTOR Q = DirectX::XMQuaternionRotationMatrix(M);
	return XMStoreQuaternion(Q);
}