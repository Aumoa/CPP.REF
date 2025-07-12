// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericWindowDefinition.h"
#include "Threading/Spinlock.h"
#include "Threading/SpinlockConditionVariable.h"
#include "Numerics/VectorInterface/Vector.h"

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

        template<class T>
        std::shared_ptr<T> GetExtension()
        {
            auto lock = std::unique_lock{ m_Lock };
            for (auto& extension : m_Extensions)
            {
                if (auto ptr = dynamic_cast<T*>(extension.get()); ptr != nullptr)
                {
                    return std::static_pointer_cast<T>(extension);
                }
            }

            return nullptr;
        }
        
        virtual GenericWindowDefinition GetDefinition() const = 0;
        virtual void* GetOSWindowHandle() const = 0;
        virtual void Show() = 0;
        virtual void Hide() = 0;
        virtual Vector2N GetSize() const = 0;
        
        void NotifyResize(const Vector2N& newSize);
    };
}