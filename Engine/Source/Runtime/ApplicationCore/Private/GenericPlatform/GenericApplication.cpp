// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "GenericPlatform/GenericApplication.h"
#include "GenericPlatform/GenericWindow.h"

namespace Ayla
{
    GenericApplication* GenericApplication::sApp;

    GenericApplication::GenericApplication()
    {
        check(!sApp);
        sApp = this;
    }

    GenericApplication::~GenericApplication() noexcept
    {
        check(sApp);
        sApp = nullptr;
    }

    String GenericApplication::GetApplicationName()
    {
        return TEXT("");
    }

    void GenericApplication::SetApplicationPointer(void* InAppPointer)
    {
        check(!bFreezed);
        ApplicationPointer = InAppPointer;
    }

    void* GenericApplication::GetApplicationPointer()
    {
        return ApplicationPointer;
    }

    void GenericApplication::Freeze()
    {
        bFreezed = true;
    }

    bool GenericApplication::IsFreezed() noexcept
    {
        return bFreezed;
    }

    void GenericApplication::QuitApplication(int32 InCode)
    {
        check(!ExitCode);
        ExitCode = InCode;
    }

    int32 GenericApplication::GetExitCode()
    {
        check(ExitCode);
        return ExitCode.value();
    }

    bool GenericApplication::IsQuitRequested()
    {
        return (bool)ExitCode;
    }
}