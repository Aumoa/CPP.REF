// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Launch.h"
#include "Engine.h"
#include "GenericPlatform/GenericApplication.h"
#include "GenericPlatform/GenericSplash.h"
#include "GenericPlatform/GenericWindow.h"
#include "Localizational/Name.h"
#include "InitializationContext.h"
#include "HAL/PlatformRenderFeature.h"
#if WITH_EDITOR
#include "EditorEngine.h"
#endif

namespace Ayla
{
    Launch::Launch()
    {
        GenericApp = GenericApplication::CreateApplication();
    }

    Launch::~Launch() noexcept
    {
    }

    int32 Launch::StartApplication()
    {
        GenericApp->SetApplicationPointer(GetApplicationPointer());

        GenericWindowDefinition wDef =
        {
            .bPrimaryWindow = true,
            .bSystemMenu = true,
            .bThickframe = true,
            .bSizebox = true,
            .bCaption = true
        };
        
#if WITH_EDITOR
        Engine = New<EditorEngine>();
#else
        Engine = New<::Ayla::Engine>();
#endif

        auto initializationContext = Engine->PreInitialize();
        Engine->Initialize(CreatePlatformRenderFeature(), initializationContext);

        auto window = GenericApp->MakeWindow(wDef);
        window->Show();

        std::vector<GenericPlatformInputEvent> InputEvents;

        while (!GenericApp->IsQuitRequested())
        {
            GenericApp->PumpMessages(InputEvents);
        }

        return GenericApp->GetExitCode();
    }
}