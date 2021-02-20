// Copyright 2020-2021 Aumoa.lib. All right reserved.

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
	using CursorMoveBindDelegate = TMulticastDelegate<void(const CursorState&, const CursorCompare&)>;

private:
	KeyboardState keys;
	KeyboardCompare keyCompare;
	CursorState cursor;
	CursorCompare cursorCompare;

	std::map<EKey, KeyActionBindDelegate> keyBinds[2];
	CursorMoveBindDelegate cursorBind;
	InputComponent* overrideComponent;

public:
	InputComponent();
	~InputComponent() override;

	void TickComponent(Seconds deltaTime) override;

	void SetOverrideComponent(InputComponent* inDerived);

	KeyActionBindDelegate& GetKeyActionBinder(EKey inKey, EKeyEvent inEventType);
	CursorMoveBindDelegate& GetCursorMoveBinder();

private:
	void UpdateKeyboardState();
	void UpdateCursorState();
};