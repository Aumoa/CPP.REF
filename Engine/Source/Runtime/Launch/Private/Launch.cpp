// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Launch.h"
#include "GenericPlatform/GenericSplash.h"
#include "GenericPlatform/GenericApplication.h"
#include "GenericPlatform/GenericWIndow.h"
#include "Localizational/Name.h"

namespace Ayla
{
    Launch::Launch(std::vector<String> CmdArgs)
        : CmdArgs(std::move(CmdArgs))
    {
        GenericApp = GenericApplication::CreateApplication();
    }

    void Launch::GatherProperties(PPtrCollection& collection)
    {
        Super::GatherProperties(collection);
        collection.Add(GenericApp);
        collection.Add(T5);
        collection.Add(T4);
        collection.Add(T3);
        collection.Add(T2);
        collection.Add(T1);
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
        
        GenericSplash::Show();
        GenericSplash::SetSplashText(TEXT("Initialize... (0/100)"));
        for (int i = 0; i < 100; ++i)
        {
            GenericSplash::SetSplashText(String::Format(TEXT("Initialize... ({0}/100)"), i + 1));
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
        GenericSplash::Hide();

        auto window = GenericApp->MakeWindow(wDef);
        window->Show();

        std::vector<GenericPlatformInputEvent> InputEvents;
        bool exit = false;
        auto thr = std::thread([this, &exit]()
        {
            while (exit == false)
            {
                T5 = std::move(T4);
                T4 = std::move(T3);
                T3 = std::move(T2);
                T2 = std::move(T1);
                std::this_thread::yield();
            }
        });

        while (!GenericApp->IsQuitRequested())
        {
            GenericApp->PumpMessages(InputEvents);
            T1 = New<Object>();
        }

        exit = true;
        thr.join();
        return GenericApp->GetExitCode();
    }
}