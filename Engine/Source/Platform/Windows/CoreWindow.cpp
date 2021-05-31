// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include <Windows.h>

import SC.Platform.Windows;
import SC.Runtime.Core;

using enum ELogVerbosity;

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

void CoreWindow::InternalCreateWindow()
{
	WNDCLASSEXW wcex = {};
	wcex.cbSize = sizeof(wcex);
	wcex.lpfnWndProc = WndProc;
	wcex.lpszClassName = L"CoreWindow";
	wcex.hInstance = GetModuleHandleW(nullptr);
	if (RegisterClassExW(&wcex) == 0)
	{
		LogSystem::Log(Fatal, 
	}
}