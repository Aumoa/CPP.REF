// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Numerics/VectorInterface/Vector.h"

namespace Ayla
{
    class IGenericWindowResizeEventHandler
    {
    protected:
        IGenericWindowResizeEventHandler()
        {
        }

        virtual ~IGenericWindowResizeEventHandler() noexcept
        {
        }

    public:
        virtual void OnResize(const Vector2N& newSize) = 0;
    };
}