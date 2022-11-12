// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#if PLATFORM_WINDOWS

#include "PlatformMisc/IPlatformWindow.h"
#include "PlatformMisc/Windows/WindowsMinimal.h"
#include "WindowsWindow.gen.h"

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
	virtual void* GetPointer() override;
	virtual Vector2N GetDrawingSize() override;

	HWND GetWindowHandle() noexcept { return _hWnd; }

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT DispatchWindowsMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#endif