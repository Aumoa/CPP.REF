// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericPlatform/GenericWindowDefinition.h"

namespace Ayla
{
    class APPLICATIONCORE_API GenericWindow : public Object
    {
    protected:
        GenericWindow();

    public:
        virtual GenericWindowDefinition GetDefinition() const = 0;
        virtual void* GetOSWindowHandle() const = 0;
        virtual void Show() = 0;
        virtual void Hide() = 0;
        virtual Vector2N GetSize() const = 0;
    };

    extern "C" APPLICATIONCORE_API void GenericWindow_Interop_Dispose(void* instancePtr);
    extern "C" APPLICATIONCORE_API void GenericWindow_Interop_Show(void* instancePtr);
    extern "C" APPLICATIONCORE_API void GenericWindow_Interop_Hide(void* instancePtr);
}