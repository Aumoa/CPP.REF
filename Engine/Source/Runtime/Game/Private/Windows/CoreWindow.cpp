// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Windows/CoreWindow.h"

#include "Windows/Application.h"
#include "Windows/WinException.h"
#include "Logging/LogMacros.h"

#define WINCHECK(Exp) if (!(Exp)) { throw WinException(); }
#define MYWINDOW if (CoreWindow* ptr = GetCoreWindow(hWnd); ptr != nullptr) (*ptr)

using namespace std;

map<HWND, CoreWindow*> CoreWindow::coreWindows;

CoreWindow::CoreWindow(Application* app) : Super()
	, app(app)
	, hWnd(nullptr)
	, bInit(false)

	, bPostQuitMessage(true)
{
	
}

CoreWindow::~CoreWindow()
{

}

void CoreWindow::Init()
{
	if (bPostQuitMessage)
	{
		Destroy += bind_delegate(OnDestroy);
	}

	Internal_CreateWindow();
	bInit = true;
}

void CoreWindow::Close()
{

}

bool CoreWindow::IsVisible_get() const
{
	return IsWindowVisible(hWnd) == TRUE;
}

void CoreWindow::IsVisible_set(bool value)
{
	ShowWindow(hWnd, value ? SW_SHOW : SW_HIDE);
}

TRefPtr<String> CoreWindow::Title_get() const
{
	int32 length = GetWindowTextLengthW(hWnd);
	wstring buffer;
	buffer.resize(length + 1);
	GetWindowTextW(hWnd, buffer.data(), length);
	return buffer;
}

void CoreWindow::Title_set(TRefPtr<String> value)
{
	SetWindowTextW(hWnd, value->C_Str);
}

void CoreWindow::Internal_RegisterClass(Application* app_internal)
{
	static atomic<bool> bRegister;

	bool bComparand = false;
	if (bool bExchanged = bRegister.compare_exchange_strong(bComparand, true); bExchanged && bComparand == false)
	{
		// Register window class.
		WNDCLASSEXW wcex = { };
		wcex.cbSize = sizeof(WNDCLASSEXA);
		wcex.lpfnWndProc = &CoreWindow::WndProc;
		wcex.lpszClassName = ClassName;
		wcex.hInstance = (HINSTANCE)app_internal->Handle;
		WINCHECK(RegisterClassExW(&wcex) != 0);
	}
}

void CoreWindow::Internal_CreateWindow()
{
	Internal_RegisterClass(app);

	// Create window.
	WINCHECK((hWnd = CreateWindowExW(
		NULL,
		ClassName,
		L"",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		nullptr,
		nullptr,
		(HINSTANCE)app->Handle,
		this
	)) != nullptr);

	Handle = hWnd;
}

LRESULT CoreWindow::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		PostInitializeCreateWindow(hWnd, lParam);
		MYWINDOW.Created.Invoke();
		break;
	case WM_DESTROY:
		MYWINDOW.Destroy.Invoke();
		PostUninitializeWindow(hWnd);
		break;
	case WM_SIZE:
		MYWINDOW.Sizing.Invoke((int16)LOWORD(lParam), (int16)HIWORD(lParam));
		MYWINDOW.PostSized.Invoke((int16)LOWORD(lParam), (int16)HIWORD(lParam));
		break;
	}

	return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

void CoreWindow::PostInitializeCreateWindow(HWND hWnd, LPARAM lParam)
{
	auto ptr = (LPCREATESTRUCTW)lParam;
	WINCHECK(ptr != nullptr);

#if defined(_DEBUG)
	if (auto it = coreWindows.find(hWnd); it != coreWindows.end())
	{
		SE_LOG(LogWindows, Fatal, L"Window is already registered.");
		return;
	}
#endif

	coreWindows.emplace(hWnd, (CoreWindow*)ptr->lpCreateParams);
}

void CoreWindow::PostUninitializeWindow(HWND hWnd)
{
	auto it = coreWindows.find(hWnd);
	if (it == coreWindows.end())
	{
		SE_LOG(LogWindows, Error, L"Window is not registered.");
		return;
	}

	coreWindows.erase(it);
}

CoreWindow* CoreWindow::GetCoreWindow(HWND hWnd)
{
	auto it = coreWindows.find(hWnd);
	if (it == coreWindows.end())
	{
		SE_LOG(LogWindows, Error, L"Window is not registered.");
		return nullptr;
	}

	return it->second;
}

void CoreWindow::OnDestroy()
{
	Application::PostQuitMessage(0);
}

#undef WINCHECK
#undef MYWINDOW