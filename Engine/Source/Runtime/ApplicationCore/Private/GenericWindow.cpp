// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "GenericWindow.h"
#include "IGenericWindowResizeEventHandler.h"
#include "IGenericWindowDestroyEventHandler.h"
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
		std::vector extensions = m_Extensions;
		lock.unlock();

		for (auto& extension : extensions)
		{
			if (auto* handler = dynamic_cast<IGenericWindowResizeEventHandler*>(extension.get()); handler)
			{
				handler->OnResize(newSize);
			}
		}
	}

	void GenericWindow::NotifyDestroy()
	{
		auto lock = std::unique_lock(m_Lock);
		std::vector extensions = m_Extensions;
		lock.unlock();

		for (auto& extension : extensions)
		{
			if (auto* handler = dynamic_cast<IGenericWindowDestroyEventHandler*>(extension.get()); handler)
			{
				handler->OnDestroy();
			}
		}
	}
}