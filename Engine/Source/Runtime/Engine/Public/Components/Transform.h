// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/Component.h"
#include "Numerics/VectorInterface/Vector.h"
#include "Numerics/TransformInterface/Quaternion.h"
#include "Numerics/MatrixInterface/Matrix4x4.h"
#include "Transform.gen.h"

namespace Ayla
{
	ACLASS()
	class ENGINE_API Transform : public Component
	{
		GENERATED_BODY()

	private:
		Vector3F m_LocalPosition = Vector3F::Zero();
		Vector3F m_LocalScale = Vector3F(1.0f, 1.0f, 1.0f);
		QuaternionF m_LocalRotation = QuaternionF::Identity();

		Transform* m_Parent = nullptr;
		mutable bool m_MatrixCached = false;
		mutable Matrix4x4F m_WorldMatrix;
		mutable Vector3F m_WorldPosition;
		mutable QuaternionF m_WorldRotation;

	public:
		ACONSTRUCTOR()
		Transform();
		virtual ~Transform() noexcept override;

		void SetParent(Transform* parent);
		Transform* GetParent() const noexcept { return m_Parent; }

		void GetLocalPositionAndRotation(Vector3F* outPosition, QuaternionF* outRotation) const noexcept
		{
			*outPosition = m_LocalPosition;
			*outRotation = m_LocalRotation;
		}

		Vector3F GetLocalPosition() const noexcept { return m_LocalPosition; }
		QuaternionF GetLocalRotation() const noexcept { return m_LocalRotation; }
		Vector3F GetLocalScale() const noexcept { return m_LocalScale; }

		void SetLocalPositionAndRotation(const Vector3F& position, const QuaternionF& rotation);
		void SetLocalPosition(const Vector3F& position);
		void SetLocalRotation(const QuaternionF& rotation);
		void SetLocalScale(const Vector3F& scale);

		void GetPositionAndRotation(Vector3F* outPosition, QuaternionF* outRotation) const noexcept
		{
			TryCacheMatrix();
			*outPosition = m_WorldPosition;
			*outRotation = m_WorldRotation;
		}

	private:
		void TryCacheMatrix() const noexcept;
	};
}