// Copyright 2020-2021 Aumoa.lib. All right reserved.

import SC.Platform.Windows;
import SC.Platform.Windows.Internal;
import SC.Runtime.Core;

using enum ELogVerbosity;

#define CALLBACK __stdcall

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
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