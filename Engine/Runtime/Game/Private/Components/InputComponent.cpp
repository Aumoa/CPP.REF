// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Components/InputComponent.h"

#include "Logging/LogMacros.h"
#include "Diagnostics/ScopedCycleCounter.h"

using namespace std;

DEFINE_STATS_GROUP(GInputComponent);

GInputComponent::GInputComponent()
	: overrideComponent(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;
}

GInputComponent::~GInputComponent()
{

}

void GInputComponent::TickComponent(Seconds deltaTime)
{
	Super::TickComponent(deltaTime);
	
	UpdateKeyboardState();
	UpdateCursorState();
}

void GInputComponent::SetOverrideComponent(GInputComponent* inDerived)
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

auto GInputComponent::GetKeyActionBinder(EKey inKey, EKeyEvent inEventType) -> KeyActionBindDelegate&
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

auto GInputComponent::GetCursorMoveBinder() -> CursorMoveBindDelegate&
{
	return cursorBind;
}

void GInputComponent::UpdateKeyboardState()
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

void GInputComponent::UpdateCursorState()
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