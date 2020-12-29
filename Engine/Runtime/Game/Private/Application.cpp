// Copyright 2020 Aumoa.lib. All right reserved.

#include "Application.h"

#include <string.h>
#include "AppDuplicationException.h"
#include "WinException.h"
#include "GameInstance.h"
#include "Engine.h"
#include "Logging/LogVerbosity.h"

using namespace SC::Runtime::Core;
using namespace SC::Runtime::Game;
using namespace SC::Runtime::Game::Logging;
using namespace std;

#define ALLOC_BREAK_NUMBER 0

Application* Application::instance;
LogCategoryBase Application::LogApplication(ELogVerbosity::Verbose, nameof(LogApplication));

Application::Application() : Super()
	, hWnd(nullptr)
{
	SetCrtMemLeakDebug();
	CheckDuplicationAndAlloc();
	CreateWindow();
}

Application::~Application()
{

}

void Application::PostInitialize()
{
	gameInstance->Initialize();
	engineLoop->PostInitialize();
}

HWND Application::GetCoreHwnd() const
{
	return hWnd;
}

GameInstance* Application::GetGameInstance() const
{
	return gameInstance.Get();
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

int32 Application::RunInternal(function<TRefPtr<GameInstance>()> objectConstructor)
{
	InitializeEngine();
	gameInstance = objectConstructor();
	PostInitialize();

	gameInstance->BeginPlay();

	SetWindowTextW(hWnd, gameInstance->ToString()->C_Str);
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

	gameInstance->EndPlay();

	return 0;
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

void Application::CreateWindow()
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
		L"GameApp",
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