// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "MovementComponent.h"

class GAME_API PawnMovementComponent : public MovementComponent
{
public:
	using Super = MovementComponent;
	using This = PawnMovementComponent;

private:
	Vector3 pendingInputVector;

public:
	PawnMovementComponent();
	~PawnMovementComponent() override;

	virtual void AddInputVector(const Vector3& inWorldAccel);

	Vector3 GetPendingInputVector() const;
	Vector3 ConsumePendingInputVector();
};