// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Numerics/TransformInterface/Quaternion.h"

#if PLATFORM_WINDOWS
#include "DirectXInlineHelper.h"

namespace Ayla
{
	QuaternionF SIMDQuaternion::FromAxisAngle(const Vector3F& Axis, DegreesF Angle)
	{
		RadiansF Rad = Angle.ToRadians();
		DirectX::XMVECTOR XAxis = XMLoadVector3(&Axis);
		DirectX::XMVECTOR XQ = DirectX::XMQuaternionRotationNormal(XAxis, Rad.Value);
		return XMStoreQuaternion(XQ);
	}

	QuaternionF SIMDQuaternion::LookTo(const Vector3F& Forward, const Vector3F& Up)
	{
		QuaternionF T;

		Vector3F XF = Vector<>::Normalize(Forward);
		Vector3F XU = Vector<>::Normalize(Up);
		Vector3F XR = Vector<>::Normalize(Vector<>::Cross(Up, Forward));
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
}

#endif