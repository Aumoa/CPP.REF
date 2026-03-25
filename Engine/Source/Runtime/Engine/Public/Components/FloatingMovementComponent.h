// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/MovementComponent.h"
#include "FloatingMovementComponent.gen.h"

namespace Ayla
{
	ACLASS()
	class ENGINE_API FloatingMovementComponent : public MovementComponent
	{
		GENERATED_BODY()

	private:
		float m_MaxSpeed = 1200.0f;
		float m_Acceleration = 4000.0f;
		float m_Deceleration = 8000.0f;
		float m_TurningBoost = 8.0f;

	public:
		ACONSTRUCTOR()
		FloatingMovementComponent();
		virtual ~FloatingMovementComponent() noexcept override;

		virtual void Tick(TickTiming timing, const TimeSpan& deltaTime) override;
		virtual float GetMaxSpeed() const override { return m_MaxSpeed; }

	private:
		void ApplyControlInputToVelocity(const TimeSpan& deltaTime, Vector3F* velocity) const;
	};
}