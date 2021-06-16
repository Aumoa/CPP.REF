// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:KeyboardTracker;

import SC.Runtime.Core;
import :KeyboardState;
import :Key;
import :BitfieldInputChangeTracker;

export class KeyboardTracker : virtual public Object
{
public:
	using Super = Object;

private:
	BitfieldInputChangeTracker<KeyboardState> _tracker;

public:
	inline KeyboardTracker()
	{
	}

	inline void Update(const KeyboardState& newState)
	{
		_tracker.Update(newState);
	}

	inline void Reset()
	{
		_tracker.Reset();
	}

	inline const KeyboardState& GetLast() const
	{
		return _tracker.GetLast();
	}

	inline bool IsKeyPressed(EKey key) const
	{
		return _tracker.IsPressedByBitIndex((uint32)key);
	}

	inline bool IsKeyReleased(EKey key) const
	{
		return _tracker.IsReleasedByBitIndex((uint32)key);
	}
};