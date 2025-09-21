// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#if PLATFORM_WINDOWS

#include "CoreMinimal.h"
#include "GenericActivity.h"
#include "WindowsActivity.gen.h"

namespace Ayla
{
	class WindowsWindow;

	ACLASS()
	class WindowsActivity : public GenericActivity
	{
		GENERATED_BODY()

	private:
		std::shared_ptr<WindowsWindow> m_MainWindow;

	public:
		WindowsActivity();
		virtual ~WindowsActivity() noexcept override;

		virtual void BeforeInitialize_Implementation() override;
		virtual void AfterInitialize_Implementation() override;
		virtual std::shared_ptr<GenericWindow> GetMainWindow_Implementation() const override;
	};
}

#endif