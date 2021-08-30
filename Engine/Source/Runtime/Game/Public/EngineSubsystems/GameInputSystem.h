// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameEngineSubsystem.h"
#include "Misc/MouseStateTracker.h"
#include "Misc/KeyboardTracker.h"
#include <optional>

class GAME_API GameInputSystem : public GameEngineSubsystem
{
	CLASS_BODY(GameInputSystem)

public:
	struct MouseMoveData
	{
		int32 CurX = 0;
		int32 CurY = 0;
		int32 DeltaX = 0;
		int32 DeltaY = 0;
	};

private:
	static MouseStateTracker _mouseTracker;
	static KeyboardTracker _keyboardTracker;
	static std::optional<int32> _lastMouseX;
	static std::optional<int32> _lastMouseY;

public:
	DECLARE_MULTICAST_EVENT(KeyboardEvent, EKey, EKeyboardEvent);
	KeyboardEvent Keyboard;
	DECLARE_MULTICAST_EVENT(MouseEvent, EMouseButton, EMouseButtonEvent);
	MouseEvent Mouse;
	DECLARE_MULTICAST_EVENT(MouseMoveEvent, const MouseMoveData&);
	MouseMoveEvent MouseMove;

public:
	GameInputSystem();
	virtual ~GameInputSystem() override;

	virtual void Tick(std::chrono::duration<float> elapsedTime) override;
};