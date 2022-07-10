// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#if PLATFORM_WINDOWS

#include "PlatformMisc/IPlatformWindow.h"
#include "WindowsWindow.generated.h"

#pragma push_macro("TEXT")
#undef TEXT
#include <Windows.h>
#pragma pop_macro("TEXT")

SCLASS()
class WindowsWindow : virtual public Object, virtual public IPlatformWindow
{
	GENERATED_BODY()

private:
	class constructor_t
	{
	public:
		constructor_t();
	};

	static constructor_t _static_ctor;

private:
	HWND _hWnd = nullptr;

public:
	WindowsWindow(const String& title);

	virtual void Show(bool bShow) override;
	virtual void Destroy() override;

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT DispatchWindowsMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#endif