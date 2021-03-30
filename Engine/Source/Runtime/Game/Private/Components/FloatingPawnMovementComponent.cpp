// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Components/FloatingPawnMovementComponent.h"

#include "Framework/Pawn.h"
#include "Framework/Controller.h"
#include "Logging/LogMacros.h"

GFloatingPawnMovementComponent::GFloatingPawnMovementComponent() : Super()
	, MaxSpeed(12.0f)
	, Acceleration(40.0f)
	, Deceleration(80.0f)
	, TurningBoost(8.0f)
{

}

GFloatingPawnMovementComponent::~GFloatingPawnMovementComponent()
{

}

void GFloatingPawnMovementComponent::TickComponent(Seconds deltaTime)
{
	Super::TickComponent(deltaTime);

	if (UpdatedComponent == nullptr)
	{
		// If updated component is nullptr, we need not calculate movement tick.
		// Abort.
		return;
	}

	// Calculate movement velocity for current frame.
	CalcVelocity(deltaTime);

	// Get delta of this frame for moving my actor.
	MoveActor(deltaTime);
}

void GFloatingPawnMovementComponent::CalcVelocity(Seconds deltaTime)
{
	Vector3 ControlAcceleration = ConsumePendingInputVectorWithMaxLength(1.0f);

	const float AnalogInputModifier = (ControlAcceleration.LengthSq > 0.f ? ControlAcceleration.Length : 0.f);
	const float MaxPawnSpeed = MaxSpeed * AnalogInputModifier;
	const bool bExceedingMaxSpeed = IsExceedingMaxSpeed(MaxPawnSpeed);

	if (AnalogInputModifier > 0.f && !bExceedingMaxSpeed)
	{
		// Apply change in velocity direction
		if (Velocity.LengthSq > 0.f)
		{
			// Change direction faster than only using acceleration, but never increase velocity magnitude.
			const float TimeScale = Math::Clamp(deltaTime.Value * TurningBoost, 0.f, 1.f);
			Velocity = Velocity + (ControlAcceleration * Velocity.Length - Velocity) * TimeScale;
		}
	}
	else
	{
		// Dampen velocity magnitude based on deceleration.
		if (Velocity.LengthSq > 0.f)
		{
			const Vector3 OldVelocity = Velocity;
			const float VelSize = Math::Max(Velocity.Length - Math::Abs(Deceleration) * deltaTime.Value, 0.f);
			Velocity = Velocity.Normalized * VelSize;

			// Don't allow braking to lower us below max speed if we started above it.
			if (bExceedingMaxSpeed && Velocity.LengthSq < Math::Square(MaxPawnSpeed))
			{
				Velocity = OldVelocity.Normalized * MaxPawnSpeed;
			}
		}
	}

	// Apply acceleration and clamp velocity magnitude.
	const float NewMaxSpeed = (IsExceedingMaxSpeed(MaxPawnSpeed)) ? Velocity.Length : MaxPawnSpeed;
	Velocity += ControlAcceleration * Math::Abs(Acceleration) * deltaTime.Value;
	Velocity = Velocity.GetClampedToMaxLength(NewMaxSpeed);
}

void GFloatingPawnMovementComponent::MoveActor(Seconds deltaTime)
{
	Vector3 delta = Velocity * deltaTime.Value;
	if (delta.NearlyEquals(Vector3::Zero, Math::SmallNumber<>))
	{
		return;
	}

	const Vector3 oldLocation = UpdatedComponent->ComponentLocation;
	const Quaternion oldRotation = UpdatedComponent->ComponentRotation;

	UpdatedComponent->MoveComponent(delta, oldRotation);

	const Vector3 newLocation = UpdatedComponent->ComponentLocation;
	Velocity = ((newLocation - oldLocation) / deltaTime.Value);
}

Vector3 GFloatingPawnMovementComponent::ConsumePendingInputVectorWithMaxLength(float inMaxLength)
{
	Vector3 pendingInput = ConsumePendingInputVector();
	if (float length = pendingInput.Length; length > 1.0f)
	{
		pendingInput /= length;
	}
	return pendingInput;
}