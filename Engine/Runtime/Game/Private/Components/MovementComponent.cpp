// Copyright 2020 Aumoa.lib. All right reserved.

#include "Components/MovementComponent.h"

MovementComponent::MovementComponent() : Super()
	, bAutoRegisterUpdatedComponent(false)
	, updatedComponent(nullptr)

	, Velocity(Vector3::Zero)
{
	PrimaryComponentTick.bCanEverTick = true;
}

MovementComponent::~MovementComponent()
{

}

void MovementComponent::TickComponent(Seconds deltaTime)
{
	Super::TickComponent(deltaTime);
}

bool MovementComponent::IsExceedingMaxSpeed(float inMaxSpeed) const
{
	inMaxSpeed = Math::Max(0.f, inMaxSpeed);
	const float MaxSpeedSquared = Math::Square(inMaxSpeed);

	// Allow 1% error tolerance, to account for numeric imprecision.
	const float OverVelocityPercent = 1.01f;
	return (Velocity.LengthSq > MaxSpeedSquared * OverVelocityPercent);
}

SceneComponent* MovementComponent::UpdatedComponent_get() const
{
	return updatedComponent;
}

void MovementComponent::UpdatedComponent_set(SceneComponent* value)
{
	updatedComponent = value;
}

bool MovementComponent::AutoRegisterUpdatedComponent_get() const
{
	return bAutoRegisterUpdatedComponent;
}

void MovementComponent::AutoRegisterUpdatedComponent_set(bool value)
{
	bAutoRegisterUpdatedComponent = value;
}