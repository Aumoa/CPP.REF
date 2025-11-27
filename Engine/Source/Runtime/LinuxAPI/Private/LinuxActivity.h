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
        SharedPtr<LinuxWindow> m_MainWindow;

    public:
        LinuxActivity();
        virtual ~LinuxActivity() noexcept override;

        virtual void BeforeInitialize_Implementation() override;
        virtual void AfterInitialize_Implementation() override;
        virtual SharedPtr<GenericWindow> GetMainWindow_Implementation() const override;

        virtual void SetTitle(String title) override;
    };
}

#endif