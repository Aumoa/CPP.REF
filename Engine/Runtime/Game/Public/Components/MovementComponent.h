// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "ActorComponent.h"

class SceneComponent;

class GAME_API MovementComponent : public ActorComponent
{
public:
	using Super = ActorComponent;
	using This = MovementComponent;

private:
	bool bAutoRegisterUpdatedComponent : 1;
	SceneComponent* updatedComponent;

public:
	MovementComponent();
	~MovementComponent() override;

	void TickComponent(Seconds deltaTime) override;
	
	bool IsExceedingMaxSpeed(float inMaxSpeed) const;

	vs_property(SceneComponent*, UpdatedComponent);
	vs_property(bool, AutoRegisterUpdatedComponent);

	Vector3 Velocity;
};