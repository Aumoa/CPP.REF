// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "WindowsIncludes.h"
#include "WindowsApplicationInterface.h"
#include "D3D11RHI/WindowsDevice.h"

DEFINE_LOG_CATEGORY(LogWindowsLaunch);

SWindowsApplicationInterface* SWindowsApplicationInterface::GApp = nullptr;

SWindowsApplicationInterface::SWindowsApplicationInterface(HINSTANCE hInstance, std::wstring_view CmdLine) : Super()
	, hInstance(hInstance)
	, CmdLine(CmdLine)
{
	GApp = this;

	WCHAR ModuleName[256] = {};
	GetModuleFileNameW((HMODULE)hInstance, ModuleName, (DWORD)std::size(ModuleName));
	SetName(ModuleName);

	WNDCLASSEXW Wcex = {};
	Wcex.cbSize = sizeof(Wcex);
	Wcex.lpszClassName = L"WindowsApplicationInterface";
	Wcex.lpfnWndProc = WndProc;
	Wcex.hInstance = hInstance;

	ATOM ClassAtom = RegisterClassExW(&Wcex);
	if (ClassAtom == 0)
	{
		ReportWindowsError(L"Could not register window class.");
	}

	hWnd = CreateWindowExW(NULL, (LPCWSTR)ClassAtom, L"AppEngine", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, hInstance, this);
	if (hWnd == nullptr)
	{
		ReportWindowsError(L"Could not create core window.");
	}
}

std::wstring SWindowsApplicationInterface::GetCommandLine()
{
	return CmdLine;
}

void SWindowsApplicationInterface::Start()
{
	ShowWindow(hWnd, SW_SHOW);

	MSG M;
	while (true)
	{
		if (GetMessageW(&M, nullptr, NULL, NULL))
		{
			TranslateMessage(&M);
			DispatchMessageW(&M);
			Idle.Invoke();
		}
		else
		{
			break;
		}
	}
}

Vector2N SWindowsApplicationInterface::GetViewportSize()
{
	RECT ClientRect;
	GetClientRect(hWnd, &ClientRect);
	return Vector2N(ClientRect.right - ClientRect.left, ClientRect.bottom - ClientRect.top);
}

IRHIDevice* SWindowsApplicationInterface::CreateDevice()
{
	return NewObject<SWindowsDevice>(hWnd);
}

LRESULT CALLBACK SWindowsApplicationInterface::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SIZE:
		GApp->Sized.Invoke(Vector2N((int32)LOWORD(lParam), (int32)HIWORD(lParam)));
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}