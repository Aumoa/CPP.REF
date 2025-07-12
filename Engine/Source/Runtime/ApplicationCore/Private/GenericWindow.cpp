// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "GenericWindow.h"
#include "IGenericWindowResizeEventHandler.h"
#include "GenericWindowExtension.h"

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

	void GenericWindow::NotifyResize(const Vector2N& newSize)
	{
		auto lock = std::unique_lock(m_Lock);
		auto extensions = m_Extensions;
		lock.unlock();

		for (auto& extension : extensions)
		{
			if (auto* handler = dynamic_cast<IGenericWindowResizeEventHandler*>(extension.get()); handler)
			{
				handler->OnResize(newSize);
			}
		}
	}
}