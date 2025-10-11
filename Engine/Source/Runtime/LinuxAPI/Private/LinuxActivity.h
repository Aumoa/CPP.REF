// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#if PLATFORM_LINUX

#include "CoreMinimal.h"
#include "GenericActivity.h"
#include "LinuxActivity.gen.h"

namespace Ayla
{
    class LinuxWindow;

    ACLASS()
    class LinuxActivity : public GenericActivity
    {
        GENERATED_BODY()

    private:
        std::shared_ptr<LinuxWindow> m_MainWindow;

    public:
        LinuxActivity();
        virtual ~LinuxActivity() noexcept override;

        virtual void BeforeInitialize_Implementation() override;
        virtual void AfterInitialize_Implementation() override;
        virtual std::shared_ptr<GenericWindow> GetMainWindow_Implementation() const override;
    };
}

#endif