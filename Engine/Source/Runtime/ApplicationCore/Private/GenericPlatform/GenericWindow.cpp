// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "GenericPlatform/GenericWindow.h"

namespace Ayla
{
	GenericWindow::GenericWindow()
	{
	}

	GenericWindow::~GenericWindow() noexcept
	{
	}

	void GenericWindow::AddExtension(std::shared_ptr<GenericWindowExtension> extension)
	{
		auto lock = std::unique_lock(m_Lock);
		m_Extensions.emplace_back(std::move(extension));
	}
}