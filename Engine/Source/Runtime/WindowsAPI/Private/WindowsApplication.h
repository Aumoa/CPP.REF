// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#define __ALLOW_PLATFORM_COMMON_H__

#include "CoreMinimal.h"
#include "GenericApplication.h"
#include "Platform/PlatformCommon.h"
#include "WindowsApplication.gen.h"

namespace Ayla
{
	ACLASS()
	class WindowsApplication : public GenericApplication
	{
		GENERATED_BODY()
		
#if PLATFORM_WINDOWS
		
	public:
		ACONSTRUCTOR()
		WindowsApplication();
		virtual ~WindowsApplication() noexcept override;

		virtual SharedPtr<GenericActivity> CreateMainActivity_Implementation() override;
		virtual SharedPtr<GenericWindow> MakeWindow(const GenericWindowDefinition& winDef) override;
		virtual Vector2N GetScreenResolution() override;
		virtual void PumpMessages(std::vector<GenericPlatformInputEvent>& outInputEvents) override;
		virtual DirectoryReference GetEngineDirectory() const override;
		virtual DirectoryReference GetApplicationDirectory() const override;
		virtual void* GetApplicationPointer() const override;
		virtual std::span<const char* const> GetVulkanExtensionNames() const override;

	private:
		static std::vector<GenericPlatformInputEvent> InputEvents;
		
#endif
	};
}

#undef __ALLOW_PLATFORM_COMMON_H__
