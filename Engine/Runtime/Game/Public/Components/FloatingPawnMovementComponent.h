// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "PawnMovementComponent.h"

class GAME_API FloatingPawnMovementComponent : public PawnMovementComponent
{
public:
	using Super = PawnMovementComponent;
	using This = FloatingPawnMovementComponent;

public:
	FloatingPawnMovementComponent();
	~FloatingPawnMovementComponent() override;

	void TickComponent(Seconds deltaTime) override;

	float MaxSpeed;
	float Acceleration;
	float Deceleration;
	float TurningBoost;

private:
	void CalcVelocity(Seconds deltaTime);
	void MoveActor(Seconds deltaTime);

	Vector3 ConsumePendingInputVectorWithMaxLength(float inMaxLength);
};