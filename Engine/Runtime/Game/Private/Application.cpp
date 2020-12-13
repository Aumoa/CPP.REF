// Copyright 2020 Aumoa.lib. All right reserved.

#include "Application.h"

#include "AppDuplicationException.h"
#include "WinException.h"

using namespace SC::Runtime::Core;
using namespace SC::Runtime::Game;

Application* Application::instance;

Application::Application(TRefPtr<String> appName) : Super()
	, hWnd(nullptr)
{
	SetCrtMemLeakDebug();
	CheckDuplicationAndAlloc();
	CreateWindow(appName);

	// TEMPORAL IMPLEMENT
	ShowWindow(hWnd, SW_SHOW);
	while (true)
	{

	}
}

Application::~Application()
{

}

Application* Application::GetInstance()
{
	return instance;
}

void Application::SetCrtMemLeakDebug()
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
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

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProcW(hWnd, uMsg, wParam, lParam);
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
	if (!hWnd)
	{
		throw WinException();
	}
}