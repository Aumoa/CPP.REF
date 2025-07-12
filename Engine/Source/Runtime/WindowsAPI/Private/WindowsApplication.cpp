// Copyright 2020-2025 Aumoa.lib. All right reserved.

#if PLATFORM_WINDOWS

#include "WindowsApplication.h"
#include "IO/FileReference.h"
#include "IO/DirectoryReference.h"
#include "WindowsWindow.h"

namespace Ayla
{
	std::vector<GenericPlatformInputEvent> WindowsApplication::InputEvents;

	WindowsApplication::WindowsApplication()
	{
	}

	WindowsApplication::~WindowsApplication() noexcept
	{
	}

	std::shared_ptr<GenericWindow> WindowsApplication::MakeWindow(const GenericWindowDefinition& winDef)
	{
		return std::make_shared<WindowsWindow>(winDef);
	}

	Vector2N WindowsApplication::GetScreenResolution()
	{
		HMONITOR monitor = MonitorFromWindow(nullptr, MONITOR_DEFAULTTOPRIMARY);
		MONITORINFO info = { sizeof(MONITORINFO) };
		if (GetMonitorInfo(monitor, &info))
		{
			return Vector2N(info.rcMonitor.right - info.rcMonitor.left,
						   info.rcMonitor.bottom - info.rcMonitor.top);
		}
		return Vector2N(0, 0);
	}

	void WindowsApplication::PumpMessages(std::vector<GenericPlatformInputEvent>& outInputEvents)
	{
		MSG msg;
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
			if (msg.message == WM_QUIT)
			{
				break;
			}
		}
		std::swap(InputEvents, outInputEvents);
		InputEvents.clear();
	}

	DirectoryReference WindowsApplication::GetEngineDirectory() const
	{
		wchar_t exePath[MAX_PATH];
		if (GetModuleFileNameW(nullptr, exePath, MAX_PATH))
		{
			FileReference exeFile(String::FromLiteral(exePath));
			return exeFile.GetDirectory().GetParent().GetParent().GetParent().GetAbsolute();
		}
		return DirectoryReference();
	}

	void* WindowsApplication::GetApplicationPointer() const
	{
		return GetModuleHandleW(nullptr);
	}

	std::span<const char* const> WindowsApplication::GetVulkanExtensionNames() const
	{
		static constexpr std::array<const char*, 2> kExtensions
		{
			"VK_KHR_surface",
			"VK_KHR_win32_surface"
		};
		return std::span(kExtensions);
	}
}

DEFINE_CREATE_GENERIC_APPLICATION(::Ayla::WindowsApplication);

#endif