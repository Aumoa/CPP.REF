// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponent.h"
#include <set>
#include <optional>
#include "GameEnums.h"
#include "PlatformMisc/MouseStateTracker.h"
#include "PlatformMisc/KeyboardTracker.h"

class InputComponent : public ActorComponent
{
public:
	using Super = ActorComponent;

public:
	using KeyboardEventDelegate = MulticastEvent<InputComponent, void(EKey, EKeyboardEvent)>;
	KeyboardEventDelegate KeyboardEvent;
	using MouseEventDelegate = MulticastEvent<InputComponent, void(EMouseButton, EMouseButtonEvent)>;
	MouseEventDelegate MouseEvent;
	using MouseMoveEventDelegate = MulticastEvent<InputComponent, void(int32 x, int32 y, int32 dx, int32 dy)>;
	MouseMoveEventDelegate MouseMoveEvent;

private:
	static std::set<InputComponent*> _inputComponents;

public:
	InputComponent();

	virtual void BeginPlay() override;
	virtual void EndPlay() override;

private:
	static MouseStateTracker _mouseTracker;
	static KeyboardTracker _keyboardTracker;
	static std::optional<int32> _lastMouseX;
	static std::optional<int32> _lastMouseY;

public:
	static void StaticTick(std::chrono::duration<float> elapsedTime);

private:
	static void BroadcastMouseEventForAllComponents(uint32 idx, EMouseButtonEvent buttonEvent);
	static void BroadcastKeyboardEventForAllComponents(uint32 idx, EKeyboardEvent keyboardEvent);
	static void BroadcastMouseMoveEventForAllComponents(int32 x, int32 y, int32 dx, int32 dy);
};