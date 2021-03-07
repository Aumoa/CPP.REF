// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Components/SpringArmComponent.h"

#include "Logging/LogMacros.h"

const TRefPtr<String> GSpringArmComponent::SocketName = L"SpringArmSocket";

GSpringArmComponent::GSpringArmComponent() : Super()
	, socketRelativeLocation(Vector3::Zero)
	, springArmLength(1.0f)
	, TargetOffset(Vector3::Zero)
	, SocketOffset(Vector3::Zero)
{
	PrimaryComponentTick.bCanEverTick = true;
}

GSpringArmComponent::~GSpringArmComponent()
{

}

void GSpringArmComponent::TickComponent(Seconds deltaTime)
{
	Super::TickComponent(deltaTime);
	UpdateSpringArmTransform(deltaTime);
}

Transform GSpringArmComponent::GetSocketTransform(TRefPtr<String> socketName, EComponentTransformSpace space) const
{
	if (socketName != SocketName)
	{
		return Super::GetSocketTransform(socketName, space);
	}

	Transform relativeTransform = Transform::Identity;
	relativeTransform.Translation = socketRelativeLocation;

	switch (space)
	{
	case EComponentTransformSpace::World:
		return Transform::Multiply(ComponentTransform, relativeTransform);
	}

	return relativeTransform;
}

float GSpringArmComponent::SpringArmLength_get() const
{
	return springArmLength;
}

void GSpringArmComponent::SpringArmLength_set(float value)
{
	if (value < 0)
	{
		SE_LOG(LogCamera, Warning, L"SpringArmLength value must be not less than zero. It will be setted to zero.");
		value = 0;
	}

	springArmLength = value;
}

void GSpringArmComponent::UpdateSpringArmTransform(Seconds deltaTime)
{
	socketRelativeLocation = Vector3(-springArmLength, 0, 0);
	socketRelativeLocation += TargetOffset;
	socketRelativeLocation += SocketOffset;

	UpdateChildTransforms();
}