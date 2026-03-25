// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ScriptableBehavior.h"
#include "Numerics/VectorInterface/Vector.h"
#include "MovementComponent.gen.h"

namespace Ayla
{
	ACLASS()
	class ENGINE_API MovementComponent : public ScriptableBehavior
	{
		GENERATED_BODY()

	private:
		Vector3F m_Velocity;

	public:
		MovementComponent();
		virtual ~MovementComponent() noexcept override;

		virtual void Tick(TickTiming timing, const TimeSpan& deltaTime) override;

		virtual float GetMaxSpeed() const { return 0; }
		virtual bool IsExceedingMaxSpeed(float maxSpeed) const;

		void SetVelocity(const Vector3F& velocity) { m_Velocity = velocity; }
		const Vector3F& GetVelocity() const { return m_Velocity; }

	};
}