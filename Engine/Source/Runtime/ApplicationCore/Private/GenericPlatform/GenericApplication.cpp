// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "GenericPlatform/GenericApplication.h"

NGenericApplication* NGenericApplication::sApp;

NGenericApplication::NGenericApplication()
{
    check(!sApp);
    sApp = this;
}

NGenericApplication::~NGenericApplication() noexcept
{
    check(sApp);
    sApp = nullptr;
}

String NGenericApplication::GetApplicationName()
{
    return TEXT("");
}

void NGenericApplication::SetApplicationPointer(void* InAppPointer)
{
    check(!bFreezed);
    ApplicationPointer = InAppPointer;
}

void* NGenericApplication::GetApplicationPointer()
{
    return ApplicationPointer;
}

void NGenericApplication::Freeze()
{
    bFreezed = true;
}

bool NGenericApplication::IsFreezed() noexcept
{
    return bFreezed;
}

void NGenericApplication::QuitApplication(int32 InCode)
{
    check(!ExitCode);
    ExitCode = InCode;
}

int32 NGenericApplication::GetExitCode()
{
    check(ExitCode);
    return ExitCode.value();
}

bool NGenericApplication::IsQuitRequested()
{
    return (bool)ExitCode;
}