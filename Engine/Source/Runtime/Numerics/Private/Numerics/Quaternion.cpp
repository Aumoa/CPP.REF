// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Numerics/TransformInterface/Quaternion.h"

#if PLATFORM_WINDOWS
#include "DirectXInlineHelper.h"

Quaternion Quaternion::FromAxisAngle(const Vector3& Axis, Degrees Angle)
{
	Radians Rad = Angle.ToRadians();
	DirectX::XMVECTOR XAxis = XMLoadVector3(&Axis);
	DirectX::XMVECTOR XQ = DirectX::XMQuaternionRotationNormal(XAxis, Rad.Value);
	return XMStoreQuaternion(XQ);
}

Quaternion Quaternion::LookTo(const Vector3& Forward, const Vector3& Up)
{
	Quaternion T;

	Vector3 XF = Vector<>::Normalize(Forward);
	Vector3 XU = Vector<>::Normalize(Up);
	Vector3 XR = Vector<>::Normalize(Vector<>::Cross(Up, Forward));
	XU = Vector<>::Cross(XF, XR);

	DirectX::XMMATRIX XM = DirectX::XMMatrixSet(
		XR.X, XR.Y, XR.Z, 0,
		XU.X, XU.Y, XU.Z, 0,
		XF.X, XF.Y, XF.Z, 0,
		0, 0, 0, 1.0f
	);

	DirectX::XMVECTOR XQ = DirectX::XMQuaternionRotationMatrix(XM);
	return XMStoreQuaternion(XQ);
}

#endif