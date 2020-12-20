// Copyright 2020 Aumoa.lib. All right reserved.

#include "Application.h"

#include "AppDuplicationException.h"
#include "WinException.h"
#include <string.h>
#include "Engine.h"
#include "Logging/LogVerbosity.h"

using namespace SC::Runtime::Core;
using namespace SC::Runtime::Game;
using namespace SC::Runtime::Game::Logging;

#define ALLOC_BREAK_NUMBER 0

Application* Application::instance;
LogCategoryBase Application::LogApplication(ELogVerbosity::Verbose, nameof(LogApplication));

Application::Application(TRefPtr<String> appName) : Super()
	, hWnd(nullptr)
{
	SetCrtMemLeakDebug();
	CheckDuplicationAndAlloc();
	CreateWindow(appName);
}

Application::~Application()
{

}

void Application::PostInitialize()
{

}

int32 Application::Run()
{
	InitializeEngine();
	PostInitialize();

	ShowWindow(hWnd, SW_SHOW);

	MSG msg{ };

	bMainLoop = true;
	while (bMainLoop)
	{
		if (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
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
		else
		{
			OnIdle();
		}
	}

	return 0;
}

HWND Application::GetCoreHwnd() const
{
	return hWnd;
}

Application* Application::GetInstance()
{
	return instance;
}

LRESULT CALLBACK Application::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SIZE:
	{
		int32 width = (int16)LOWORD(lParam);
		int32 height = (int16)HIWORD(lParam);
		GApplication.PreSizing.Invoke(width, height);
		GApplication.PostSized.Invoke(width, height);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

void Application::SetCrtMemLeakDebug()
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

#if ALLOC_BREAK_NUMBER != 0
	_CrtSetBreakAlloc(ALLOC_BREAK_NUMBER);
#endif

#endif
}

void Application::CheckDuplicationAndAlloc()
{
	if (instance != nullptr)
	{
		throw AppDuplicationException();
	}

	instance = this;
}

void Application::CreateWindow(TRefPtr<String> appName)
{
	WNDCLASSEXW wcex{ };
	wcex.cbSize = sizeof(wcex);
	wcex.lpfnWndProc = WndProc;
	wcex.lpszClassName = L"SC::Runtime::Game::Application";
	wcex.hInstance = GetModuleHandleW(nullptr);

	if (RegisterClassExW(&wcex) == 0)
	{
		throw WinException();
	}

	hWnd = CreateWindowExW(
		NULL,
		wcex.lpszClassName,
		appName->C_Str,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		nullptr,
		nullptr,
		wcex.hInstance,
		nullptr
	);

	if (hWnd == nullptr)
	{
		throw WinException();
	}
}

void Application::InitializeEngine()
{
	engineLoop = NewObject<Engine>();
	engineLoop->Initialize();
}

void Application::OnIdle()
{
	engineLoop->Tick();
}