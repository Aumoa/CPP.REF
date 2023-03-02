// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericPlatform/GenericWindowDefinition.h"

class NGenericWindow;

class APPLICATIONCORE_API NGenericApplication
{
private:
    static NGenericApplication* sApp;
    void* ApplicationPointer = nullptr;
    bool bFreezed = false;

protected:
    NGenericApplication();

public:
    virtual ~NGenericApplication() noexcept;

    virtual std::unique_ptr<NGenericWindow> MakeWindow(const NGenericWindowDefinition& InDefinition) = 0;
    virtual Vector2N GetScreenResolution() = 0;

    virtual String GetApplicationName();

    virtual void SetApplicationPointer(void* InAppPointer);
    virtual void* GetApplicationPointer();

    virtual void Freeze();
    bool IsFreezed() noexcept;

public:
    static std::unique_ptr<NGenericApplication> CreateApplication();
    static NGenericApplication* GetApplication() noexcept { return sApp; }
};