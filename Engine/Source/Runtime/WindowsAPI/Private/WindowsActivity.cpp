// Copyright 2020-2025 Aumoa.lib. All right reserved.

#if PLATFORM_WINDOWS

#include "WindowsActivity.h"
#include "WindowsApplication.h"
#include "WindowsWindow.h"
#include "GenericMainWindowDefaultExt.h"

namespace Ayla
{
	WindowsActivity::WindowsActivity()
	{
		m_MainWindow = std::static_pointer_cast<WindowsWindow>(GenericApplication::Get().MakeWindow(GenericWindowDefinition
		{
			.Caption = TEXT("Main Activity"),
			.bThickframe = true,
			.DesiredScreenPosition = Vector2N(-1, -1),
			.DesiredScreenSize = Vector2N(-1, -1),
		}));

		m_MainWindow->AddExtension(New<GenericMainWindowDefaultExt>());
	}

	WindowsActivity::~WindowsActivity() noexcept
	{
	}

	void WindowsActivity::BeforeInitialize_Implementation()
	{
	}

	void WindowsActivity::AfterInitialize_Implementation()
	{
		m_MainWindow->Show();
	}

	std::shared_ptr<GenericWindow> WindowsActivity::GetMainWindow_Implementation() const
	{
		return m_MainWindow;
	}
}

#endif