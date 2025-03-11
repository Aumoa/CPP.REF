// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericPlatform/GenericWindowDefinition.h"
#include "GenericPlatform/GenericPlatformInputEvent.h"

class NGenericWindow;

class APPLICATIONCORE_API NGenericApplication
{
private:
    static NGenericApplication* sApp;
    void* ApplicationPointer = nullptr;
    bool bFreezed = false;
    std::optional<int32> ExitCode;

protected:
    NGenericApplication();

public:
    virtual ~NGenericApplication() noexcept;

    virtual std::unique_ptr<NGenericWindow> MakeWindow(const NGenericWindowDefinition& InDefinition) = 0;
    virtual Vector2N GetScreenResolution() = 0;
    virtual void PumpMessages(std::vector<NGenericPlatformInputEvent>& OutInputEvents) = 0;

    virtual String GetApplicationName();
    virtual DirectoryReference GetEngineDirectory() const = 0;

    virtual void SetApplicationPointer(void* InAppPointer);
    virtual void* GetApplicationPointer();

    virtual void Freeze();
    bool IsFreezed() noexcept;

    void QuitApplication(int32 InCode);
    bool IsQuitRequested();
    int32 GetExitCode();

public:
    static std::unique_ptr<NGenericApplication> CreateApplication();
    static NGenericApplication& Get() noexcept { return *sApp; }
};

extern "C" APPLICATIONCORE_API void* GenericApplication_Interop_CreateApplication();
extern "C" APPLICATIONCORE_API void GenericApplication_Interop_Dispose(void* instancePtr);
extern "C" APPLICATIONCORE_API void* GenericApplication_Interop_MakeWindow(void* instancePtr);