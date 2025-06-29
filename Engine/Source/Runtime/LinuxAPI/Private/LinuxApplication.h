// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericPlatform/GenericApplication.h"

#if PLATFORM_LINUX

namespace Ayla
{
    class LinuxApplication : public GenericApplication
    {
    public:
        LinuxApplication();

        virtual std::shared_ptr<GenericWindow> MakeWindow(const GenericWindowDefinition& winDef) override;
        virtual Vector2N GetScreenResolution() override;
        virtual void PumpMessages(std::vector<GenericPlatformInputEvent>& outInputEvents) override;
        virtual DirectoryReference GetEngineDirectory() const override;

    private:
        static std::vector<GenericPlatformInputEvent> InputEvents;
    };
}

#endif
