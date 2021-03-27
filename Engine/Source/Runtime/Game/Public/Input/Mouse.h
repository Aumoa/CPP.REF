// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "TBitfieldInputChangeTracker.h"

enum class EMouseButton
{
	Left,
	Middle,
	Right,
	X1,
	X2
};

enum class EMouseButtonEvent
{
	Pressed,
	Released,
};

enum class EMousePositionMode
{
	Absolute,
	Relative
};

struct MouseState
{
	bool bLeftButton : 1;
	bool bMiddleButton : 1;
	bool bRightButton : 1;
	bool bXButton1 : 1;
	bool bXButton2 : 1;
	int32 X;
	int32 Y;
	int32 ScrollWheelValue;
	EMousePositionMode Mode;
};

class GAME_API MouseStateTracker : virtual public Object
{
public:
	using Super = Object;

private:
	TBitfieldInputChangeTracker<MouseState> tracker;

public:
	MouseStateTracker();
	~MouseStateTracker() override;

	void Update(const MouseState& inNewState);
	void Reset();
	vs_property_get_auto(MouseState, Last, tracker.Last);

	bool IsButtonPressed(EMouseButton inButton) const;
	bool IsButtonReleased(EMouseButton inButton) const;
};