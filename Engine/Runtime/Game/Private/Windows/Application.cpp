// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Windows/Application.h"

#include "Windows/CoreWindow.h"
#include "Windows/WinKeyboard.h"
#include "Windows/WinMouse.h"
#include "Logging/LogMacros.h"

Application* Application::instance;

Application::Application()
{
	hModule = (HANDLE)GetModuleHandleW(nullptr);
	Handle = hModule;
}

Application::~Application()
{

}

inline void ProcessInputMessage(uint32 message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_ACTIVATEAPP:
		    WinKeyboard::ProcessMessage(message, wParam, lParam);
			WinMouse::ProcessMessage(message, wParam, lParam);
		    break;
		
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
		    WinKeyboard::ProcessMessage(message, wParam, lParam);
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
			WinMouse::ProcessMessage(message, wParam, lParam);
			break;
	}
}

void Application::Run(TRefPtr<CoreWindow> main)
{
	if (instance != nullptr)
	{
		SE_LOG(LogWindows, Fatal, L"Application duplication detected.");
		return;
	}

	instance = this;

	// Initialize application.
	mainWindow = main;
	if (!main->IsInitialized)
	{
		main->Init();
	}
	Initialize();

	winKey = NewObject<WinKeyboard>();
	winMouse = NewObject<WinMouse>();
	winMouse->SetWindow(main->HWnd);

	// Show main windows.
	if (!main->IsVisible)
	{
		main->IsVisible = true;
	}

	MSG msg = { };
	bMainLoop = true;
	while (bMainLoop)
	{
		bool bMsgAvailable = PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE);
		if (bMsgAvailable)
		{
			if (msg.message == WM_QUIT)
			{
				bMainLoop = false;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessageW(&msg);
				ProcessInputMessage(msg.message, msg.wParam, msg.lParam);
			}
		}

		if (bForcedTick || !bMsgAvailable)
		{
			Tick();
		}
	}

	Shutdown();
}

void Application::Tick()
{

}

void Application::PostQuitMessage(int32 exitCode)
{
	::PostQuitMessage(exitCode);
}

Application* Application::GetInstance()
{
	return instance;
}

void Application::Initialize()
{

}

void Application::Shutdown()
{

}