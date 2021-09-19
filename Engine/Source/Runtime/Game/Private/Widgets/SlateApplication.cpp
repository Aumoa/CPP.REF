// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Widgets/SlateApplication.h"
#include "GameEngine.h"
#include "EngineSubsystems/GameInputSystem.h"

SSlateApplication::SSlateApplication(const std::wstring& name) : Super(name)
{
}

SSlateApplication::~SSlateApplication()
{
}

void SSlateApplication::Init()
{
	auto system = GEngine->GetEngineSubsystem<SGameInputSystem>();
	checkf(system, L"GameInputSystem does not initialized.");

	system->Mouse.AddSObject(this, &SSlateApplication::OnMouse);
	system->Keyboard.AddSObject(this, &SSlateApplication::OnKeyboard);
}

void SSlateApplication::OnMouse(Vector2N location, EMouseButton button, EMouseButtonEvent event)
{
	for (auto& widget : GetWidgets())
	{
		if (widget->SendMouseEvent(MakeRootGeometry(), location, button, event))
		{
			return;
		}
	}
}

void SSlateApplication::OnKeyboard(EKey key, EKeyboardEvent event)
{
	for (auto& widget : GetWidgets())
	{
		if (widget->SendKeyboardEvent(MakeRootGeometry(), key, event))
		{
			return;
		}
	}
}