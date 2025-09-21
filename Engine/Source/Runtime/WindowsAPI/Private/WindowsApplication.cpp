// Copyright 2020-2025 Aumoa.lib. All right reserved.

#if PLATFORM_WINDOWS

#include "WindowsApplication.h"
#include "WindowsWindow.h"
#include "WindowsActivity.h"
#include "IO/FileReference.h"
#include "IO/DirectoryReference.h"

namespace Ayla
{
	std::vector<GenericPlatformInputEvent> WindowsApplication::InputEvents;

	WindowsApplication::WindowsApplication()
	{
	}

	WindowsApplication::~WindowsApplication() noexcept
	{
	}

	std::shared_ptr<GenericActivity> WindowsApplication::CreateMainActivity_Implementation()
	{
		return New<WindowsActivity>();
	}

	std::shared_ptr<GenericWindow> WindowsApplication::MakeWindow(const GenericWindowDefinition& winDef)
	{
		return New<WindowsWindow>(winDef);
	}

	Vector2N WindowsApplication::GetScreenResolution()
	{
		return Vector2N(
			GetSystemMetrics(SM_CXSCREEN),
			GetSystemMetrics(SM_CYSCREEN)
		);
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