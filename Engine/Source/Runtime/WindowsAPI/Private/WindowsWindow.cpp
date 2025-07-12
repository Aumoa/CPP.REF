// Copyright 2020-2025 Aumoa.lib. All right reserved.

#if PLATFORM_WINDOWS

#include "WindowsWindow.h"

namespace Ayla
{
	WindowsWindow::WindowsWindow(const GenericWindowDefinition& winDef)
	{
		m_CachedDefinition = winDef;

		WNDCLASSW wc = {};
		wc.lpfnWndProc = DefWindowProcW;
		wc.hInstance = GetModuleHandle(nullptr);
		wc.lpszClassName = L"AylaWindowClass";
		if (!RegisterClassW(&wc))
		{
			throw std::runtime_error("Failed to register window class.");
		}

		DWORD style = WS_OVERLAPPEDWINDOW;
		if (!winDef.bThickframe)
		{
			style &= ~WS_CAPTION & ~WS_THICKFRAME;
		}

		static auto makepos = [](int32 pos) -> int
		{
			return pos < 0 ? CW_USEDEFAULT : pos;
		};

		m_WindowHandle = CreateWindowExW(
			0, wc.lpszClassName, L"",
			style, makepos(winDef.DesiredScreenPosition.X), makepos(winDef.DesiredScreenPosition.Y),
			makepos(winDef.DesiredScreenSize.X), makepos(winDef.DesiredScreenSize.Y),
			nullptr, nullptr, wc.hInstance, nullptr
		);
		if (!m_WindowHandle)
		{
			throw std::runtime_error("Failed to create window.");
		}
	}

	WindowsWindow::~WindowsWindow() noexcept
	{
		if (m_WindowHandle)
		{
			DestroyWindow(m_WindowHandle);
			m_WindowHandle = nullptr;
		}
	}

	GenericWindowDefinition WindowsWindow::GetDefinition() const
	{
		return m_CachedDefinition;
	}

	void* WindowsWindow::GetOSWindowHandle() const
	{
		return m_WindowHandle;
	}

	void WindowsWindow::Show()
	{
		if (m_WindowHandle)
		{
			ShowWindow(m_WindowHandle, SW_SHOW);
			UpdateWindow(m_WindowHandle);
		}
	}

	void WindowsWindow::Hide()
	{
		if (m_WindowHandle)
		{
			ShowWindow(m_WindowHandle, SW_HIDE);
		}
	}

	Vector2N WindowsWindow::GetSize() const
	{
		if (m_WindowHandle)
		{
			RECT rect;
			if (GetClientRect(m_WindowHandle, &rect))
			{
				return Vector2N(rect.right - rect.left, rect.bottom - rect.top);
			}
		}
		return Vector2N(0, 0);
	}
}

#endif