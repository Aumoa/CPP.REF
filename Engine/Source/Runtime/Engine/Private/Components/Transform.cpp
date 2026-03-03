// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Components/Transform.h"
#include "LogEngine.h"

namespace Ayla
{
	Transform::Transform() : Super()
	{
	}

	Transform::~Transform() noexcept
	{
	}

	void Transform::SetParent(Transform* parent)
	{
		if (m_Parent != parent)
		{
			m_Parent = parent;
			m_MatrixCached = false;
		}
	}

	void Transform::SetLocalPositionAndRotation(const Vector3F& position, const QuaternionF& rotation)
	{
		if (position != m_LocalPosition || rotation != m_LocalRotation)
		{
			m_LocalPosition = position;
			m_LocalRotation = rotation;
			m_MatrixCached = false;
		}
	}

	void Transform::SetLocalScale(const Vector3F& scale)
	{
		if (m_LocalScale != scale)
		{
			m_LocalScale = scale;
			m_MatrixCached = false;
		}
	}

	void Transform::TryCacheMatrix() const noexcept
	{
		if (m_MatrixCached)
		{
			return;
		}

		auto localMatrix = Matrix4x4<>::AffineTransformation(Translate3D<float>(m_LocalPosition), Scale3D<float>(m_LocalScale), m_LocalRotation);
		if (m_Parent == nullptr)
		{
			m_WorldMatrix = localMatrix;
			m_WorldPosition = m_LocalPosition;
			m_WorldRotation = m_LocalRotation;
		}
		else
		{
			m_Parent->TryCacheMatrix();
			for (size_t i = 0; i < 4; ++i)
			{
				auto& v = m_Parent->m_WorldMatrix[i];
				PlatformProcess::OutputDebugString(String::Format(TEXT("{:.6f} {:.6f} {:.6f} {:.6f}\n"), v[0], v[1], v[2], v[3]));
			}
			m_WorldMatrix = m_Parent->m_WorldMatrix * localMatrix;
			m_WorldPosition = m_Parent->m_WorldMatrix.TransformPoint(m_LocalPosition);
			m_WorldRotation = m_Parent->m_WorldRotation.Multiply(m_LocalRotation);
		}

		m_MatrixCached = true;
	}
}