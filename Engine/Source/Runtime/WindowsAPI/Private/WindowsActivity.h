// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#if PLATFORM_WINDOWS

#include "CoreMinimal.h"
#include "GenericActivity.h"

namespace Ayla
{
	class WindowsWindow;

	class WindowsActivity : public GenericActivity
	{
	private:
		std::shared_ptr<WindowsWindow> m_MainWindow;

	public:
		WindowsActivity();
		virtual ~WindowsActivity() noexcept override;

		virtual void BeforeInitialize() override;
		virtual void AfterInitialize() override;
		virtual std::shared_ptr<GenericWindow> GetMainWindow() const override;
	};
}

#endif