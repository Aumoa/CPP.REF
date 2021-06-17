// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:InputComponent;

import std.core;
import :ActorComponent;
import :MouseStateTracker;
import :KeyboardTracker;
import :Key;
import :KeyboardEvent;
import :MouseButton;
import :MouseButtonEvent;

using namespace std;
using namespace std::chrono;

export class InputComponent : public ActorComponent
{
public:
	using Super = ActorComponent;

public:
	using KeyboardEventDelegate = MulticastEvent<InputComponent, void(EKey, EKeyboardEvent)>;
	KeyboardEventDelegate KeyboardEvent;
	using MouseEventDelegate = MulticastEvent<InputComponent, void(EMouseButton, EMouseButtonEvent)>;
	MouseEventDelegate MouseEvent;

private:
	static set<InputComponent*> _inputComponents;

public:
	InputComponent();

	virtual void BeginPlay() override;
	virtual void EndPlay() override;

private:
	static MouseStateTracker _mouseTracker;
	static KeyboardTracker _keyboardTracker;

public:
	static void StaticTick(duration<float> elapsedTime);

private:
	static void BroadcastMouseEventForAllComponents(uint32 idx, EMouseButtonEvent buttonEvent);
	static void BroadcastKeyboardEventForAllComponents(uint32 idx, EKeyboardEvent keyboardEvent);
};