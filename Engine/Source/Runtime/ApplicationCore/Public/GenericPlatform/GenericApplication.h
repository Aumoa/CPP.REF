// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericPlatform/GenericWindowDefinition.h"
#include "GenericPlatform/GenericPlatformInputEvent.h"
#include "GenericPlatform/GenericWindow.h"
#include "IO/DirectoryReference.h"
#include "GenericApplication.gen.h"

namespace Ayla
{
    class GenericWindow;

    ACLASS()
    class APPLICATIONCORE_API GenericApplication : public Object
    {
        GENERATED_BODY()

    private:
        static GenericApplication* sApp;
        void* ApplicationPointer = nullptr;
        bool bFreezed = false;
        std::optional<int32> ExitCode;

    protected:
        GenericApplication();
        virtual void Finalize() override;

    public:
        virtual RPtr<GenericWindow> MakeWindow(const GenericWindowDefinition& InDefinition) = 0;
        virtual Vector2N GetScreenResolution() = 0;
        virtual void PumpMessages(std::vector<GenericPlatformInputEvent>& OutInputEvents) = 0;

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
        static RPtr<GenericApplication> CreateApplication();
        static GenericApplication& Get() noexcept { return *sApp; }
    };
}