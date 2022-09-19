// Copyright 2020-2022 Aumoa.lib. All right reserved.

#if PLATFORM_WINDOWS

#include "Casts.h"
#include "CoreAssert.h"
#include "PlatformMisc/Windows/WindowsWindow.h"
#include "Threading/Spinlock.h"
#include "WindowsWindow.gen.cpp"

#define GWLP_SELF GWLP_USERDATA

WindowsWindow::constructor_t WindowsWindow::_static_ctor;

WindowsWindow::constructor_t::constructor_t()
{
	WNDCLASSEXW wcex =
	{
		.cbSize = sizeof(WNDCLASSEXW),
		.lpfnWndProc = WndProc,
		.cbWndExtra = sizeof(void*),
		.hInstance = GetModuleHandleW(nullptr),
		.lpszClassName = L"WindowsWindow"
	};
	if (RegisterClassExW(&wcex) == 0)
	{
		checkf(false, TEXT("Cannot register window class."));
	}
}

WindowsWindow::WindowsWindow(const String& title)
{
	CreateWindowExW(0,
		L"WindowsWindow",
		title.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		nullptr,
		nullptr,
		GetModuleHandleW(nullptr),
		Cast<void*>(this)
	);
}

void WindowsWindow::Show(bool bShow)
{
	check(_hWnd);
	ShowWindow(_hWnd, bShow ? SW_SHOW : SW_HIDE);
}

void WindowsWindow::Destroy()
{
	if (_hWnd)
	{
		DestroyWindow(_hWnd);
		_hWnd = nullptr;
	}
}

void* WindowsWindow::GetPointer()
{
	return _hWnd;
}

Vector2N WindowsWindow::GetDrawingSize()
{
	RECT rc;
	if (!GetClientRect(_hWnd, &rc))
	{
		return Vector2N::Zero();
	}
	else
	{
		return Vector2N(rc.right - rc.left, rc.bottom - rc.top);
	}
}

LRESULT CALLBACK WindowsWindow::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_NCCREATE)
	{
		auto lpCreateParams = (LPCREATESTRUCTW)lParam;
		SetWindowLongPtrW(hWnd, GWLP_SELF, (LONG_PTR)lpCreateParams->lpCreateParams);
	}

	auto* self = Cast<WindowsWindow>((void*)GetWindowLongPtrW(hWnd, GWLP_SELF));
	if (self)
	{
		return self->DispatchWindowsMessage(hWnd, uMsg, wParam, lParam);
	}
	else
	{
		return DefWindowProcW(hWnd, uMsg, wParam, lParam);
	}
}

LRESULT WindowsWindow::DispatchWindowsMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_NCCREATE)
	{
		_hWnd = hWnd;
	}

	if (_hWnd)
	{
		switch (uMsg)
		{
		case WM_DESTROY:
			WindowDestroyed.Broadcast();
			break;
		}
	}

	return DefWindowProcW(hWnd, (UINT)uMsg, (WPARAM)wParam, (LPARAM)lParam);
}

#endif