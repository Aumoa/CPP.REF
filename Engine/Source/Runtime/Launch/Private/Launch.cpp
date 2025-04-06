// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Launch.h"
#include "GenericPlatform/GenericSplash.h"
#include "GenericPlatform/GenericApplication.h"
#include "GenericPlatform/GenericWIndow.h"
#include "Localizational/Name.h"
#include "InitializationContext.h"
#if WITH_EDITOR
#include "EditorEngine.h"
#endif

namespace Ayla
{
    Launch::Launch(std::vector<String> CmdArgs)
        : CmdArgs(std::move(CmdArgs))
    {
        GenericApp = GenericApplication::CreateApplication();
    }

    int32 Launch::GuardedMain()
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
        Engine->Initialize(initializationContext);

        auto window = GenericApp->MakeWindow(wDef);
        window->Show();

        std::vector<GenericPlatformInputEvent> InputEvents;

        try
        {
            while (!GenericApp->IsQuitRequested())
            {
                GenericApp->PumpMessages(InputEvents);
                New<Object>();
            }
        }
        catch (Exception* e)
        {
            delete e;
            throw;
        }

        return GenericApp->GetExitCode();
    }
}