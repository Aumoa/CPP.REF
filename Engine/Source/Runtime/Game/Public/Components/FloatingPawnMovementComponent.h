// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "PawnMovementComponent.h"

class GAME_API GFloatingPawnMovementComponent : public GPawnMovementComponent
{
public:
	using Super = GPawnMovementComponent;
	using This = GFloatingPawnMovementComponent;

public:
	GFloatingPawnMovementComponent();
	~GFloatingPawnMovementComponent() override;

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