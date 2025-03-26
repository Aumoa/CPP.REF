// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Launch.h"
#include "GenericPlatform/GenericSplash.h"
#include "GenericPlatform/GenericApplication.h"
#include "GenericPlatform/GenericWIndow.h"
#include "Localizational/Name.h"

#if WITH_EDITOR
#include "CoreEd.h"
#endif

namespace Ayla
{
    NLaunch* NLaunch::CurrentLaunch;

    NLaunch::NLaunch(String CmdArgs)
        : CmdArgs(CmdArgs)
    {
        check(CurrentLaunch == nullptr);
        CurrentLaunch = this;

        GenericApp = NGenericApplication::CreateApplication();
    }

    NLaunch::~NLaunch() noexcept
    {
        check(CurrentLaunch);
        CurrentLaunch = nullptr;
    }

    int32 NLaunch::GuardedMain()
    {
        GenericApp->SetApplicationPointer(GetApplicationPointer());

        NGenericWindowDefinition wDef =
        {
            .bPrimaryWindow = true,
            .bSystemMenu = true,
            .bThickframe = true,
            .bSizebox = true,
            .bCaption = true
        };

        NGenericSplash::Show();
        NGenericSplash::SetSplashText(TEXT("Initialize... (0/100)"));
        for (int i = 0; i < 100; ++i)
        {
            NGenericSplash::SetSplashText(String::Format(TEXT("Initialize... ({0}/100)"), i + 1));
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        NGenericSplash::Hide();

        std::shared_ptr window = GenericApp->MakeWindow(wDef);
        window->Show();

        std::vector<NGenericPlatformInputEvent> InputEvents;
        while (!GenericApp->IsQuitRequested())
        {
            GenericApp->PumpMessages(InputEvents);
        }

        return GenericApp->GetExitCode();
    }

    NLaunch& NLaunch::Get() noexcept
    {
        return *CurrentLaunch;
    }
}

extern "C" LAUNCH_API ::Ayla::int32 Ayla__Launch__StartApplication(const ::Ayla::char_t* const* args, ::Ayla::int32 length)
{
    using namespace ::Ayla;

    std::vector<String> Args;
    for (int i = 0; i < length; ++i)
    {
        Args.emplace_back(String::FromLiteral(args[i]));
    }

    return NLaunch::GeneratePlatformLaunch(String::Join(TEXT(" "), Args))->GuardedMain();
}