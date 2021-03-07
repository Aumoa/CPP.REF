// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "ActorComponent.h"

#include "PlatformMisc/PlatformInput.h"
#include "Key.h"

class GAME_API GInputComponent : public GActorComponent
{
public:
	using Super = GActorComponent;
	using This = GInputComponent;

	using KeyActionBindDelegate = TMulticastDelegate<void(EKey, EKeyEvent)>;
	using CursorMoveBindDelegate = TMulticastDelegate<void(const CursorState&, const CursorCompare&)>;

private:
	KeyboardState keys;
	KeyboardCompare keyCompare;
	CursorState cursor;
	CursorCompare cursorCompare;

	std::map<EKey, KeyActionBindDelegate> keyBinds[2];
	CursorMoveBindDelegate cursorBind;
	GInputComponent* overrideComponent;

public:
	GInputComponent();
	~GInputComponent() override;

	void TickComponent(Seconds deltaTime) override;

	void SetOverrideComponent(GInputComponent* inDerived);

	KeyActionBindDelegate& GetKeyActionBinder(EKey inKey, EKeyEvent inEventType);
	CursorMoveBindDelegate& GetCursorMoveBinder();

private:
	void UpdateKeyboardState();
	void UpdateCursorState();
};