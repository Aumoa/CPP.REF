// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "SceneComponent.h"

class GAME_API SpringArmComponent : public SceneComponent
{
public:
	using Super = SceneComponent;
	using This = SpringArmComponent;

public:
	static const TRefPtr<String> SocketName;

	Vector3 socketRelativeLocation;

	float springArmLength;

public:
	SpringArmComponent();
	~SpringArmComponent() override;

	void TickComponent(Seconds deltaTime) override;

	Transform GetSocketTransform(TRefPtr<String> socketName, EComponentTransformSpace space = EComponentTransformSpace::World) const override;

	vs_property(float, SpringArmLength);
	Vector3 TargetOffset;
	Vector3 SocketOffset;

private:
	void UpdateSpringArmTransform(Seconds deltaTime);
};