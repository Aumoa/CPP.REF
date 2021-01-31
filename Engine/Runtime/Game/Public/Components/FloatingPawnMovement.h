// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "PawnMovementComponent.h"

class GAME_API FloatingPawnMovement : public PawnMovementComponent
{
public:
	using Super = PawnMovementComponent;
	using This = FloatingPawnMovement;

public:
	FloatingPawnMovement();
	~FloatingPawnMovement() override;

	void TickComponent(Seconds deltaTime) override;

	float MaxSpeed;
	float Acceleration;
	float Deceleration;
	float TurningBoost;

private:
	void CalcVelocity(Seconds deltaTime);
	void MoveActor(const Vector3& delta);

	Vector3 ConsumePendingInputVectorWithMaxLength(float inMaxLength);
};