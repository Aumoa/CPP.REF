// Copyright 2020 Aumoa.lib. All right reserved.

#include "Components/SpringArmComponent.h"

using namespace std;
using namespace std::chrono;

TRefPtr<const String> SpringArmComponent::SocketName = L"SpringArmSocket";

SpringArmComponent::SpringArmComponent() : Super()
	, socketRelativeLocation(Vector3::Zero)
	, springArmLength(100.0f)
	, targetOffset(Vector3::Zero)
	, socketOffset(Vector3::Zero)
{
	PrimaryComponentTick.bCanEverTick = true;
}

SpringArmComponent::~SpringArmComponent()
{

}

void SpringArmComponent::TickComponent(Seconds deltaTime)
{
	Super::TickComponent(deltaTime);
	UpdateSpringArmTransform(deltaTime);
}

Transform SpringArmComponent::GetSocketTransform(TRefPtr<String> socketName, EComponentTransformSpace space) const
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

void SpringArmComponent::UpdateSpringArmTransform(Seconds deltaTime)
{
	socketRelativeLocation = Vector3(-springArmLength, 0, 0);
	socketRelativeLocation += targetOffset;
	socketRelativeLocation += socketOffset;

	UpdateChildTransforms();
}