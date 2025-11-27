// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "CoreMinimal.h"
#include "GenericWindow.h"
#if PLATFORM_LINUX
#include <X11/Xlib.h>
#endif
#include "LinuxWindow.gen.h"

namespace Ayla
{
    ACLASS()
    class LinuxWindow : public GenericWindow
    {
        GENERATED_BODY()

#if PLATFORM_LINUX
        Display* m_Display = nullptr;
        Window m_Window;
        GenericWindowDefinition m_CachedDefinition;

    public:
        LinuxWindow(Display* display, const GenericWindowDefinition& winDef);
        virtual ~LinuxWindow() noexcept override;

        virtual GenericWindowDefinition GetDefinition() const override;
        virtual void* GetOSWindowHandle() const override;
        virtual void Show() override;
        virtual void Hide() override;
        virtual Vector2N GetSize() const override;

        void OnDestroy();
        void SetName(String text);
#endif
    };
}

