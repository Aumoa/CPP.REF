// Copyright 2020-2025 Aumoa.lib. All right reserved.

#if PLATFORM_WINDOWS

#include "MainWindowExt.h"
#include "GenericApplication.h"

namespace Ayla
{
	MainWindowExt::MainWindowExt()
	{
	}

	MainWindowExt::~MainWindowExt() noexcept
	{
	}

	void MainWindowExt::OnDestroy()
	{
		auto& app = GenericApplication::Get();
		app.QuitApplication(0);
	}
}

#endif