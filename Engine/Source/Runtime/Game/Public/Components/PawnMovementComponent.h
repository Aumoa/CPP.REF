// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "MovementComponent.h"

class GAME_API GPawnMovementComponent : public GMovementComponent
{
public:
	using Super = GMovementComponent;
	using This = GPawnMovementComponent;

private:
	Vector3 pendingInputVector;

public:
	GPawnMovementComponent();
	~GPawnMovementComponent() override;

	virtual void AddInputVector(const Vector3& inWorldAccel);

	Vector3 GetPendingInputVector() const;
	Vector3 ConsumePendingInputVector();
};