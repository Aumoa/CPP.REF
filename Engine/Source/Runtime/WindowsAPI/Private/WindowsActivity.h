// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

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
		
#if PLATFORM_WINDOWS

	private:
		SharedPtr<WindowsWindow> m_MainWindow;

	public:
		WindowsActivity();
		virtual ~WindowsActivity() noexcept override;

		virtual void BeforeInitialize_Implementation() override;
		virtual void AfterInitialize_Implementation() override;
		virtual SharedPtr<GenericWindow> GetMainWindow_Implementation() const override;

#endif
	};
}