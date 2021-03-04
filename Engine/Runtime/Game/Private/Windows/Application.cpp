// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Windows/Application.h"

#include "Windows/CoreWindow.h"
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