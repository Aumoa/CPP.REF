// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Components/MovementComponent.h"
#include "Numerics/VectorInterface/Vector.h"

namespace Ayla
{
	MovementComponent::MovementComponent()
	{
		m_PrimaryTick.bCanEverTick = true;
	}

	MovementComponent::~MovementComponent() noexcept
	{
	}

	void MovementComponent::Tick(TickTiming timing, const TimeSpan& deltaTime)
	{
		Super::Tick(timing, deltaTime);
	}

	bool MovementComponent::IsExceedingMaxSpeed(float maxSpeed) const
	{
		maxSpeed = Math::Max(0, maxSpeed);
		const float maxSpeedSq = maxSpeed * maxSpeed;

		constexpr float kTolerance = 1.01f;
		return Vector<>::LengthSq(GetVelocity()) > maxSpeedSq * kTolerance;
	}
}