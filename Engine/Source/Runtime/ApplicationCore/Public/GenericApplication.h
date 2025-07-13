// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericWindowDefinition.h"
#include "GenericPlatformInputEvent.h"
#include "GenericWindow.h"
#include "IO/DirectoryReference.h"

namespace Ayla
{
    class GenericWindow;
    class GenericActivity;

    class APPLICATIONCORE_API GenericApplication
    {
    private:
        static GenericApplication* sApp;
        bool bFreezed = false;
        std::optional<int32> ExitCode;

    protected:
        GenericApplication();

    public:
        virtual ~GenericApplication() noexcept;

        virtual std::shared_ptr<GenericActivity> CreateMainActivity() = 0;
        virtual std::shared_ptr<GenericWindow> MakeWindow(const GenericWindowDefinition& InDefinition) = 0;
        virtual Vector2N GetScreenResolution() = 0;
        virtual void PumpMessages(std::vector<GenericPlatformInputEvent>& OutInputEvents) = 0;

        virtual String GetApplicationName();
        virtual DirectoryReference GetEngineDirectory() const = 0;
        virtual std::span<const char* const> GetVulkanExtensionNames() const = 0;
        virtual void* GetApplicationPointer() const = 0;

        virtual void Freeze();
        bool IsFreezed() noexcept;

        void QuitApplication(int32 InCode);
        bool IsQuitRequested();
        int32 GetExitCode();

    public:
        static GenericApplication& Get() noexcept { return *sApp; }
    };
}

#define NAMEOF_CREATE_GENERIC_APPLICATION TEXT("CreateGenericApplication")

#define DEFINE_CREATE_GENERIC_APPLICATION(ClassName) \
extern "C" \
{ \
    PLATFORM_SHARED_EXPORT auto CreateGenericApplication() \
    { \
        return (::Ayla::GenericApplication*)(new ClassName()); \
    } \
}
