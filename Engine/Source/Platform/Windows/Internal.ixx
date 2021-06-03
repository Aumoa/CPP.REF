// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include <Windows.h>

export module SC.Platform.Windows.Internal;

export
{
	// Windows.h
	typedef LRESULT LRESULT;
	typedef HWND HWND;
	typedef UINT UINT;
	typedef WPARAM WPARAM;
	typedef LPARAM LPARAM;
	typedef WNDCLASSEXW WNDCLASSEXW;
	typedef MSG MSG;
	typedef INT INT;
	typedef HINSTANCE HINSTANCE;
	typedef LPWSTR LPWSTR;

	WINUSERAPI LRESULT WINAPI DefWindowProcW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	WINUSERAPI VOID WINAPI PostQuitMessage(int nExitCode);
	WINUSERAPI ATOM WINAPI RegisterClassExW(CONST WNDCLASSEXW*);
	WINUSERAPI HWND WINAPI CreateWindowExW(DWORD dwExStyle, LPCWSTR lpClassName, LPCWSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
	WINUSERAPI BOOL WINAPI ShowWindow(HWND hWnd, int nCmdShow);
	WINUSERAPI BOOL WINAPI PeekMessageW(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT uMsgFilterMax, UINT wRemoveMsg);
	WINUSERAPI BOOL WINAPI TranslateMessage(CONST MSG* lpMsg);
	WINUSERAPI LRESULT WINAPI DispatchMessageW(CONST MSG* lpMsg);

	WINBASEAPI HMODULE WINAPI GetModuleHandleW(LPCWSTR lpModuleName);
	WINBASEAPI DWORD WINAPI GetLastError();

#undef WM_DESTROY
#undef WM_QUIT
	constexpr UINT WM_DESTROY = 0x0002;
	constexpr UINT WM_QUIT = 0x0012;
#undef WS_OVERLAPPEDWINDOW
	constexpr UINT WS_OVERLAPPEDWINDOW = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
#undef CW_USEDEFAULT
	constexpr int CW_USEDEFAULT = ((int)0x80000000);
#undef SW_SHOW
	constexpr int SW_SHOW = 5;
#undef PM_REMOVE
	constexpr UINT PM_REMOVE = 0x0001;
}