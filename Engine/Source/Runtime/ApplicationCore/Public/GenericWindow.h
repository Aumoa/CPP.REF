// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericWindowDefinition.h"
#include "KeyCode.h"
#include "Threading/Spinlock.h"
#include "Threading/SpinlockConditionVariable.h"
#include "Numerics/VectorInterface/Vector.h"
#include "GenericWindow.gen.h"

namespace Ayla
{
    class GenericWindowExtension;

    ACLASS()
    class APPLICATIONCORE_API GenericWindow : public Object
    {
        GENERATED_BODY()

    private:
        Spinlock m_Lock;
        std::vector<SharedPtr<GenericWindowExtension>> m_Extensions;

    protected:
        GenericWindow();

    public:
        virtual ~GenericWindow() noexcept;

        void AddExtension(SharedPtr<GenericWindowExtension> extension);

        template<class T>
        SharedPtr<T> GetExtension()
        {
            auto lock = std::unique_lock{ m_Lock };
            for (auto& extension : m_Extensions)
            {
                if (SharedPtr<T> ptr; extension.Is(&ptr))
                {
                    return ptr;
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
        void NotifyDestroy();
        void NotifyKeyDown(KeyCode keyCode);
        void NotifyKeyUp(KeyCode keyCode);
    };
}