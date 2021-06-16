// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:MouseStateTracker;

import SC.Runtime.Core;
import :BitfieldInputChangeTracker;
import :MouseState;
import :MouseButton;

export class MouseStateTracker : virtual public Object
{
public:
	using Super = Object;

private:
	BitfieldInputChangeTracker<MouseState> _tracker;

public:
	inline MouseStateTracker()
	{
	}

	inline void Update(const MouseState& newState)
	{
		_tracker.Update(newState);
	}

	inline void Reset()
	{
		_tracker.Reset();
	}

	inline const MouseState& GetLast()
	{
		return _tracker.GetLast();
	}

	inline bool IsButtonPressed(EMouseButton inButton) const
	{
		return _tracker.IsPressedByBitIndex((uint32)inButton);
	}

	inline bool IsButtonReleased(EMouseButton inButton) const
	{
		return _tracker.IsReleasedByBitIndex((uint32)inButton);
	}
};