// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "ActorComponent.h"

#include "PlatformMisc/PlatformInput.h"
#include "Key.h"

class GAME_API InputComponent : public ActorComponent
{
public:
	using Super = ActorComponent;
	using This = InputComponent;

	using KeyActionBindDelegate = TMulticastDelegate<void(EKey, EKeyEvent)>;

private:
	KeyboardState keys;
	KeyboardCompare keyCompare;

	std::map<EKey, KeyActionBindDelegate> keyBinds[2];
	InputComponent* overrideComponent;

public:
	InputComponent();
	~InputComponent() override;

	void TickComponent(Seconds deltaTime) override;

	void SetOverrideComponent(InputComponent* inDerived);

	KeyActionBindDelegate& GetKeyActionBinder(EKey inKey, EKeyEvent inEventType);

private:
	void UpdateKeyboardState();
};