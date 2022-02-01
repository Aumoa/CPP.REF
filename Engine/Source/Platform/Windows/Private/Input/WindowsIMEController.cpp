// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "WindowsIMEController.h"

GENERATE_BODY(SWindowsIMEController);

SWindowsIMEController* SWindowsIMEController::sInstance;

SWindowsIMEController::SWindowsIMEController() : Super()
{
	checkf(sInstance == nullptr, L"Internal logic error.");
	sInstance = this;
}

void SWindowsIMEController::ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	std::optional<IMEEvent> Event;

	switch (uMsg)
	{
	case WM_CHAR:
		Event.emplace(EIMEEvent::Char);
		Event->Char.ComposedChar = (wchar_t)wParam;
		break;
	case WM_IME_STARTCOMPOSITION:
		Event.emplace(EIMEEvent::StartComposition);
		break;
	case WM_IME_ENDCOMPOSITION:
		Event.emplace(EIMEEvent::EndComposition);
		break;
	case WM_IME_COMPOSITION:
		Event.emplace(EIMEEvent::Composition);
		Event->Composition.ComposingChar = (wchar_t)wParam;
		break;
	}

	if (Event.has_value())
	{
		sInstance->IME.Broadcast(*Event);
	}
}