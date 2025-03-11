// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "GenericPlatform/GenericApplication.h"
#include "GenericPlatform/GenericWIndow.h"

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

extern "C" void* GenericApplication_Interop_CreateApplication()
{
    return NGenericApplication::CreateApplication().release();
}

extern "C" void GenericApplication_Interop_Dispose(void* instancePtr)
{
    delete (NGenericApplication*)instancePtr;
}

extern "C" void* GenericApplication_Interop_MakeWindow(void* instancePtr)
{
    NGenericWindowDefinition defaultOptions;
    defaultOptions.bPrimaryWindow = true;
    defaultOptions.bSystemMenu = true;
    defaultOptions.bThickframe = true;
    defaultOptions.bSizebox = true;
    defaultOptions.bCaption = true;
    return ((NGenericApplication*)instancePtr)->MakeWindow(defaultOptions).release();
}