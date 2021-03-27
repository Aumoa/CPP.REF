// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "ActorComponent.h"

class GSceneComponent;

class GAME_API GMovementComponent : public GActorComponent
{
public:
	using Super = GActorComponent;
	using This = GMovementComponent;

private:
	bool bAutoRegisterUpdatedComponent : 1;
	GSceneComponent* updatedComponent;

public:
	GMovementComponent();
	~GMovementComponent() override;

	void TickComponent(Seconds deltaTime) override;
	
	bool IsExceedingMaxSpeed(float inMaxSpeed) const;

	vs_property(GSceneComponent*, UpdatedComponent);
	vs_property(bool, AutoRegisterUpdatedComponent);

	Vector3 Velocity;
};