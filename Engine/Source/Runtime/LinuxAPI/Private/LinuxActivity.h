// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#if PLATFORM_LINUX

#include "CoreMinimal.h"
#include "GenericActivity.h"

namespace Ayla
{
    class LinuxWindow;

    class LinuxActivity : public GenericActivity
    {
    private:
        std::shared_ptr<LinuxWindow> m_MainWindow;

    public:
        LinuxActivity();
        virtual ~LinuxActivity() noexcept override;

        virtual void BeforeInitialize() override;
        virtual void AfterInitialize() override;
        virtual std::shared_ptr<GenericWindow> GetMainWindow() const override;
    };
}

#endif