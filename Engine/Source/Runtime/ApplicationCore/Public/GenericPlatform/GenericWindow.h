// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericPlatform/GenericWindowDefinition.h"
#include "Threading/Spinlock.h"
#include "Threading/SpinlockConditionVariable.h"

namespace Ayla
{
    class GenericWindowExtension;

    class APPLICATIONCORE_API GenericWindow
    {
    private:
        Spinlock m_Lock;
        std::vector<std::shared_ptr<GenericWindowExtension>> m_Extensions;

    protected:
        GenericWindow();

    public:
        virtual ~GenericWindow() noexcept;

        void AddExtension(std::shared_ptr<GenericWindowExtension> extension);
        
        virtual GenericWindowDefinition GetDefinition() const = 0;
        virtual void* GetOSWindowHandle() const = 0;
        virtual void Show() = 0;
        virtual void Hide() = 0;
        virtual Vector2N GetSize() const = 0;
    };
}