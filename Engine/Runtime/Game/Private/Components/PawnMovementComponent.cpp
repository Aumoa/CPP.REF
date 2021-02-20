// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Components/PawnMovementComponent.h"

using namespace std;

PawnMovementComponent::PawnMovementComponent() : Super()
	, pendingInputVector(Vector3::Zero)
{

}

PawnMovementComponent::~PawnMovementComponent()
{

}

void PawnMovementComponent::AddInputVector(const Vector3& inWorldAccel)
{
	pendingInputVector += inWorldAccel;
}

Vector3 PawnMovementComponent::GetPendingInputVector() const
{
	return pendingInputVector;
}

Vector3 PawnMovementComponent::ConsumePendingInputVector()
{
	Vector3 pending = Vector3::Zero;
	swap(pending, pendingInputVector);
	return pending;
}