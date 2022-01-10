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
		sInstance->IME.Broadcast(*Event);
		break;
	}
}