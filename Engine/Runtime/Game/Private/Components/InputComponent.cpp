// Copyright 2020 Aumoa.lib. All right reserved.

#include "Components/InputComponent.h"

#include "Logging/LogMacros.h"

using namespace std;

InputComponent::InputComponent()
	: overrideComponent(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;
}

InputComponent::~InputComponent()
{

}

void InputComponent::TickComponent(Seconds deltaTime)
{
	Super::TickComponent(deltaTime);

	UpdateKeyboardState();
	UpdateCursorState();
}

void InputComponent::SetOverrideComponent(InputComponent* inDerived)
{
	if (overrideComponent == inDerived)
	{
		// Has not changed.
		return;
	}

	if (overrideComponent != nullptr)
	{
		RemovePrerequisiteObject(overrideComponent);
		overrideComponent = nullptr;
	}

	overrideComponent = inDerived;

	if (overrideComponent != nullptr)
	{
		AddPrerequisiteObject(overrideComponent);
	}
}

auto InputComponent::GetKeyActionBinder(EKey inKey, EKeyEvent inEventType) -> KeyActionBindDelegate&
{
	if ((size_t)inEventType >= 2)
	{
		static KeyActionBindDelegate dummy;
		SE_LOG(LogInput, Error, L"Keyboard event type {0} is undefined. Abort.", (int32)inEventType);
		return dummy;
	}

	auto& myKeyBind = keyBinds[(size_t)inEventType];

	if (auto it = myKeyBind.find(inKey); it != myKeyBind.end())
	{
		return it->second;
	}
	else
	{
		auto r = myKeyBind.emplace(inKey, KeyActionBindDelegate());
		return r.first->second;
	}
}

auto InputComponent::GetCursorMoveBinder() -> CursorMoveBindDelegate&
{
	return cursorBind;
}

void InputComponent::UpdateKeyboardState()
{
	KeyboardState currentKeys = PlatformInput::GetKeyboardState();
	keyCompare.Compare(keys, currentKeys);
	keys = move(currentKeys);

	if (overrideComponent != nullptr)
	{
		for (auto& keyBind : overrideComponent->keyBinds[0])
		{
			if (keyCompare.IsKeyDown(keyBind.first))
			{
				keyBind.second.Invoke(keyBind.first, EKeyEvent::Pressed);
			}
		}

		for (auto& keyBind : overrideComponent->keyBinds[1])
		{
			if (keyCompare.IsKeyUp(keyBind.first))
			{
				keyBind.second.Invoke(keyBind.first, EKeyEvent::Released);
			}
		}
	}

	for (auto& keyBind : keyBinds[0])
	{
		if (keyCompare.IsKeyDown(keyBind.first))
		{
			keyBind.second.Invoke(keyBind.first, EKeyEvent::Pressed);
		}
	}

	for (auto& keyBind : keyBinds[1])
	{
		if (keyCompare.IsKeyUp(keyBind.first))
		{
			keyBind.second.Invoke(keyBind.first, EKeyEvent::Released);
		}
	}
}

void InputComponent::UpdateCursorState()
{
	CursorState currentCursor = PlatformInput::GetCursorState();
	cursorCompare.Compare(cursor, currentCursor);
	cursor = move(currentCursor);

	if (overrideComponent != nullptr)
	{
		overrideComponent->cursorBind.Invoke(cursor, cursorCompare);
	}

	cursorBind.Invoke(cursor, cursorCompare);
}