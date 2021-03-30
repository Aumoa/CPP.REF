// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Components/PawnMovementComponent.h"

using namespace std;

GPawnMovementComponent::GPawnMovementComponent() : Super()
	, pendingInputVector(Vector3::Zero)
{

}

GPawnMovementComponent::~GPawnMovementComponent()
{

}

void GPawnMovementComponent::AddInputVector(const Vector3& inWorldAccel)
{
	pendingInputVector += inWorldAccel;
}

Vector3 GPawnMovementComponent::GetPendingInputVector() const
{
	return pendingInputVector;
}

Vector3 GPawnMovementComponent::ConsumePendingInputVector()
{
	Vector3 pending = Vector3::Zero;
	swap(pending, pendingInputVector);
	return pending;
}