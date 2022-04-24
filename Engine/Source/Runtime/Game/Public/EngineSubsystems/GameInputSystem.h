// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "GameEngineSubsystem.h"

namespace libty::inline Game
{
	class GAME_API SGameInputSystem : public SGameEngineSubsystem
	{
		GENERATED_BODY(SGameInputSystem);

	public:
		struct MouseMoveData
		{
			int32 CurX = 0;
			int32 CurY = 0;
			int32 DeltaX = 0;
			int32 DeltaY = 0;
		};

	private:
		SPROPERTY(MouseTracker)
		SMouseStateTracker* MouseTracker = nullptr;
		SPROPERTY(KeyboardTracker)
		SKeyboardTracker* KeyboardTracker = nullptr;
		std::optional<int32> LastMouseX;
		std::optional<int32> LastMouseY;

	public:
		DECLARE_MULTICAST_EVENT(KeyboardEvent, EKey, EKeyboardEvent);
		KeyboardEvent Keyboard;
		DECLARE_MULTICAST_EVENT(MouseEvent, Vector2N, EMouseButton, EMouseButtonEvent);
		MouseEvent Mouse;
		DECLARE_MULTICAST_EVENT(MouseMoveEvent, const MouseMoveData&);
		MouseMoveEvent MouseMove;

	public:
		SGameInputSystem();

		virtual void Tick(float InDeltaTime) override;
	};
}