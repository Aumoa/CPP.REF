// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#if PLATFORM_WINDOWS

#define __ALLOW_PLATFORM_COMMON_H__

#include "CoreMinimal.h"
#include "GenericWindow.h"
#include "Platform/PlatformCommon.h"

namespace Ayla
{
	class WindowsWindow : public GenericWindow
	{
		GenericWindowDefinition m_CachedDefinition;
		HWND m_WindowHandle = nullptr;

	public:
		WindowsWindow(const GenericWindowDefinition& winDef);
		virtual ~WindowsWindow() noexcept override;

		virtual GenericWindowDefinition GetDefinition() const override;
		virtual void* GetOSWindowHandle() const override;
		virtual void Show() override;
		virtual void Hide() override;
		virtual Vector2N GetSize() const override;
	};
}

#undef __ALLOW_PLATFORM_COMMON_H__

#endif
