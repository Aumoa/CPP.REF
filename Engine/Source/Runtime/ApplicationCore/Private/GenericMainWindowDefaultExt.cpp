// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "GenericMainWindowDefaultExt.h"
#include "GenericApplication.h"

namespace Ayla
{
	GenericMainWindowDefaultExt::GenericMainWindowDefaultExt()
	{
	}

	GenericMainWindowDefaultExt::~GenericMainWindowDefaultExt() noexcept
	{
	}

	void GenericMainWindowDefaultExt::OnDestroy()
	{
		auto& app = GenericApplication::Get();
		app.QuitApplication(0);
	}
}