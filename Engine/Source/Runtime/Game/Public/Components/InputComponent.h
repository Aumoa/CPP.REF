// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "ActorComponent.h"

class KeyStateTracker;
class MouseStateTracker;

class GAME_API GInputComponent : public GActorComponent
{
public:
	using Super = GActorComponent;
	using This = GInputComponent;

private:
	TRefPtr<KeyStateTracker> keyTracker;
	TRefPtr<MouseStateTracker> mouseTracker;

public:
	GInputComponent();
	~GInputComponent() override;

	void TickComponent(Seconds deltaTime) override;

	vs_property_get_auto(KeyStateTracker*, KeyTracker, keyTracker.Get());
	vs_property_get_auto(MouseStateTracker*, MouseTracker, mouseTracker.Get());
};