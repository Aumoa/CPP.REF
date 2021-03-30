// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Components/MovementComponent.h"

GMovementComponent::GMovementComponent() : Super()
	, bAutoRegisterUpdatedComponent(false)
	, updatedComponent(nullptr)

	, Velocity(Vector3::Zero)
{
	PrimaryComponentTick.bCanEverTick = true;
}

GMovementComponent::~GMovementComponent()
{

}

void GMovementComponent::TickComponent(Seconds deltaTime)
{
	Super::TickComponent(deltaTime);
}

bool GMovementComponent::IsExceedingMaxSpeed(float inMaxSpeed) const
{
	inMaxSpeed = Math::Max(0.f, inMaxSpeed);
	const float MaxSpeedSquared = Math::Square(inMaxSpeed);

	// Allow 1% error tolerance, to account for numeric imprecision.
	const float OverVelocityPercent = 1.01f;
	return (Velocity.LengthSq > MaxSpeedSquared * OverVelocityPercent);
}

GSceneComponent* GMovementComponent::UpdatedComponent_get() const
{
	return updatedComponent;
}

void GMovementComponent::UpdatedComponent_set(GSceneComponent* value)
{
	updatedComponent = value;
}

bool GMovementComponent::AutoRegisterUpdatedComponent_get() const
{
	return bAutoRegisterUpdatedComponent;
}

void GMovementComponent::AutoRegisterUpdatedComponent_set(bool value)
{
	bAutoRegisterUpdatedComponent = value;
}