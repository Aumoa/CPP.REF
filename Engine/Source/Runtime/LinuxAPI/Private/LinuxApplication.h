// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#if PLATFORM_LINUX

#include "CoreMinimal.h"
#include "GenericApplication.h"
#include "Threading/Spinlock.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "LinuxApplication.gen.h"

namespace Ayla
{
    class LinuxWindow;

    ACLASS()
    class LinuxApplication : public GenericApplication
    {
        GENERATED_BODY()

    private:
        Display* m_Display = nullptr;
        Spinlock m_Spinlock;
        std::map<void*, std::weak_ptr<LinuxWindow>> m_WeakWindows;

    public:
        LinuxApplication();
        virtual ~LinuxApplication() noexcept override;

        virtual std::shared_ptr<GenericActivity> CreateMainActivity() override;
        virtual std::shared_ptr<GenericWindow> MakeWindow(const GenericWindowDefinition& winDef) override;
        virtual Vector2N GetScreenResolution() override;
        virtual void PumpMessages(std::vector<GenericPlatformInputEvent>& outInputEvents) override;
        virtual DirectoryReference GetEngineDirectory() const override;
        virtual void* GetApplicationPointer() const override;
        virtual std::span<const char* const> GetVulkanExtensionNames() const override;

    private:
        static std::vector<GenericPlatformInputEvent> InputEvents;
    };
}

#endif
