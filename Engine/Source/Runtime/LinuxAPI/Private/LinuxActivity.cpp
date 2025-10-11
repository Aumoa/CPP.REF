// Copyright 2020-2025 Aumoa.lib. All right reserved.

#if PLATFORM_LINUX

#include "LinuxActivity.h"
#include "LinuxApplication.h"
#include "LinuxWindow.h"
#include "GenericMainWindowDefaultExt.h"

namespace Ayla
{
    LinuxActivity::LinuxActivity()
    {
        m_MainWindow = std::static_pointer_cast<LinuxWindow>(GenericApplication::Get().MakeWindow(GenericWindowDefinition
        {
            .Caption = TEXT("Main Activity"),
            .bThickframe = true,
            .DesiredScreenPosition = Vector2N(-1, -1),
            .DesiredScreenSize = Vector2N(-1, -1)
        }));

        m_MainWindow->AddExtension(std::make_shared<GenericMainWindowDefaultExt>());
    }

	LinuxActivity::~LinuxActivity() noexcept
	{
	}

	void LinuxActivity::BeforeInitialize_Implementation()
	{
	}

	void LinuxActivity::AfterInitialize_Implementation()
	{
		m_MainWindow->Show();
	}

	std::shared_ptr<GenericWindow> LinuxActivity::GetMainWindow_Implementation() const
	{
		return m_MainWindow;
	}
}

#endif