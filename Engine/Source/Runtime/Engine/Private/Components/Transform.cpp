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

	void Transform::SetLocalPosition(const Vector3F& position)
	{
		if (m_LocalPosition != position)
		{
			m_LocalPosition = position;
			m_MatrixCached = false;
		}
	}

	void Transform::SetLocalRotation(const QuaternionF& rotation)
	{
		if (rotation != m_LocalRotation)
		{
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

	void Transform::SetPositionAndRotation(const Vector3F& position, const QuaternionF& rotation)
	{
		if (position != m_WorldPosition || rotation != m_WorldRotation)
		{
			if (m_Parent == nullptr)
			{
				SetLocalPositionAndRotation(position, rotation);
			}
			else
			{
				m_Parent->TryCacheMatrix();
				auto localPosition = Matrix4x4<>::Inverse(m_Parent->m_WorldMatrix).TransformPoint(position);
				auto localRotation = m_Parent->m_WorldRotation.Inverse().Multiply(rotation);
				SetLocalPositionAndRotation(localPosition, localRotation);
			}
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
			m_WorldMatrix = m_Parent->m_WorldMatrix * localMatrix;
			m_WorldPosition = m_Parent->m_WorldMatrix.TransformPoint(m_LocalPosition);
			m_WorldRotation = m_Parent->m_WorldRotation.Multiply(m_LocalRotation);
		}

		m_MatrixCached = true;
	}
}