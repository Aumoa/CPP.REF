// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "WindowsApplication.h"
#include "Input/WindowsPlatformKeyboard.h"
#include "Input/WindowsPlatformMouse.h"
#include "D3D12RHI/DXGIFactory.h"

#define WM_UPDATETICKMODE		WM_APP + 1
#define WM_PRESENT				WM_UPDATETICKMODE + 1

SWindowsApplication::SWindowsApplication(HINSTANCE hInstance) : Super()
	, hInstance(hInstance)
{
	check(gApp == nullptr);
	gApp = this;

	WNDCLASSEXW Wcex = {};
	Wcex.cbSize = sizeof(Wcex);
	Wcex.lpfnWndProc = WndProc;
	Wcex.lpszClassName = L"WindowsApplication";
	Wcex.hInstance = hInstance;
	if (RegisterClassExW(&Wcex) == 0)
	{
		ReportWindowsError(L"Could not register window class.");
	}

	hWnd = CreateWindowExW(0, Wcex.lpszClassName, L"WindowsApplication", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, hInstance, nullptr);
	if (hWnd == nullptr)
	{
		ReportWindowsError(L"Could not create core window.");
	}
}

void SWindowsApplication::Start()
{
	ShowWindow(hWnd, SW_SHOW);

	MSG Msg = {};

	while (true)
	{
		if (TickMode == ETickMode::Realtime)
		{
			if (PeekMessageW(&Msg, nullptr, 0, 0, PM_REMOVE))
			{
				if (Msg.message == WM_QUIT)
				{
					break;
				}
				else
				{
					TranslateMessage(&Msg);
					DispatchMessageW(&Msg);
				}
			}
			else
			{
				Idle.Invoke();
			}
		}
		else
		{
			if (!GetMessageW(&Msg, nullptr, 0, 0))
			{
				break;
			}
			TranslateMessage(&Msg);
			DispatchMessageW(&Msg);
			Idle.Invoke();
		}
	}
}

Vector2N SWindowsApplication::GetViewportSize()
{
	RECT Rc;
	GetClientRect(hWnd, &Rc);
	return Vector2N(Rc.right - Rc.left, Rc.bottom - Rc.top);
}

float SWindowsApplication::GetDpi()
{
	return (float)GetDpiForWindow(hWnd);
}

void SWindowsApplication::SetTickMode(ETickMode InTickMode)
{
	if (TickMode != InTickMode)
	{
		TickMode = InTickMode;
		if (InTickMode == ETickMode::Realtime)
		{
			SendMessageW(hWnd, WM_UPDATETICKMODE, 0, 0);
		}
	}
}

auto SWindowsApplication::GetTickMode() -> ETickMode
{
	return TickMode;
}

void SWindowsApplication::SetTitle(std::wstring_view InTitle)
{
	SetWindowTextW(hWnd, InTitle.data());
}

std::wstring SWindowsApplication::GetTitle()
{
	int32 Length = GetWindowTextLengthW(hWnd);
	std::wstring Buf;
	Buf.resize(Length);

	GetWindowTextW(hWnd, Buf.data(), Length);
	return Buf;
}

IRHIFactory* SWindowsApplication::GetFactory()
{
	if (Factory == nullptr)
	{
		Factory = NewObject<SDXGIFactory>(this);
	}
	return Factory;
}

IPlatformKeyboard& SWindowsApplication::GetPlatformKeyboard()
{
	return SWindowsPlatformKeyboard::Get();
}

IPlatformMouse& SWindowsApplication::GetPlatformMouse()
{
	return SWindowsPlatformMouse::Get();
}

HWND SWindowsApplication::GetWindowHandle()
{
	return hWnd;
}

LRESULT CALLBACK SWindowsApplication::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_ACTIVATEAPP:
		SWindowsPlatformMouse::ProcessMessage(uMsg, wParam, lParam);
		SWindowsPlatformKeyboard::ProcessMessage(uMsg, wParam, lParam);
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
		SWindowsPlatformMouse::ProcessMessage(uMsg, wParam, lParam);
		break;
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		SWindowsPlatformKeyboard::ProcessMessage(uMsg, wParam, lParam);
		break;
	case WM_SIZE:
		gApp->Sized.Invoke(Vector2N(LOWORD(lParam), HIWORD(lParam)));
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}