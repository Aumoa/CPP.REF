// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include <Windows.h>

import SC.Platform.Windows;
import SC.Runtime.Core;
import SC.Runtime.Game;

using enum ELogVerbosity;

#define CALLBACK __stdcall

inline void SetupHwndParameters(HWND hWnd, LPARAM lParam)
{
	auto lpCreateStruct = (LPCREATESTRUCTW)lParam;
	auto* coreWindow = (CoreWindow*)lpCreateStruct->lpCreateParams;

	SetPropW(hWnd, L"this", coreWindow);
}

inline CoreWindow* GetThis(HWND hWnd)
{
	return (CoreWindow*)GetPropW(hWnd, L"this");
}

inline void FinallizeHwndParameters(HWND hWnd)
{
	RemovePropW(hWnd, L"this");
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_ACTIVATEAPP:
		WinMouse::ProcessMessage(uMsg, wParam, lParam);
		WinKeyboard::ProcessMessage(uMsg, wParam, lParam);
		break;
	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
		WinMouse::ProcessMessage(uMsg, wParam, lParam);
		break;
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		WinKeyboard::ProcessMessage(uMsg, wParam, lParam);
		break;
	case WM_CREATE:
		SetupHwndParameters(hWnd, lParam);
		break;
	case WM_SIZE:
		if (CoreWindow* cw = GetThis(hWnd); cw != nullptr)
		{
			cw->Size.Invoke((int16)LOWORD(lParam), (int16)HIWORD(lParam));
		}
		break;
	case WM_DESTROY:
		FinallizeHwndParameters(hWnd);
		PostQuitMessage(0);
		break;
	}

	return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

CoreWindow::CoreWindow() : Super()
{
	WNDCLASSEXW wcex = {};
	wcex.cbSize = sizeof(wcex);
	wcex.lpfnWndProc = WndProc;
	wcex.lpszClassName = L"CoreWindow";
	wcex.hInstance = GetModuleHandleW(nullptr);
	if (RegisterClassExW(&wcex) == 0)
	{
		LogSystem::Log(LogWindows, Fatal, L"Could not register window class with error code: {}. Abort.", ::GetLastError());
		return;
	}

	HWND hWnd = CreateWindowExW(0, wcex.lpszClassName, L"GameApp", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, wcex.hInstance, this);
	if (hWnd == nullptr)
	{
		LogSystem::Log(LogWindows, Fatal, L"Could not create core window with error code: {}. Abort.", ::GetLastError());
		return;
	}

	_hwnd = hWnd;
}

void CoreWindow::Start()
{
	::ShowWindow((HWND)_hwnd, SW_SHOW);

	MSG msg{ };

	_bMainLoop = true;
	while (_bMainLoop)
	{
		if (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				_bMainLoop = false;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessageW(&msg);
			}
		}
		else
		{
			Idle.Invoke();
		}
	}
}

void* CoreWindow::GetWindowHandle() const
{
	return _hwnd;
}

int32 CoreWindow::GetLastError() const
{
	return _lastError;
}

void CoreWindow::SetLastError(int32 code)
{
	_lastError = code;
}