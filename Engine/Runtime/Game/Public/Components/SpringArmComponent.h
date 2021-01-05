// Copyright 2020 Aumoa.lib. All right reserved.

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
	static TRefPtr<const String> SocketName;

	Vector3 socketRelativeLocation;

	float springArmLength;
	Vector3 targetOffset;
	Vector3 socketOffset;

public:
	SpringArmComponent();
	~SpringArmComponent() override;

	void TickComponent(std::chrono::duration<double> deltaTime) override;

	Transform GetSocketTransform(TRefPtr<String> socketName, EComponentTransformSpace space = EComponentTransformSpace::World) const override;

private:
	void UpdateSpringArmTransform(std::chrono::duration<double> deltaTime);
};