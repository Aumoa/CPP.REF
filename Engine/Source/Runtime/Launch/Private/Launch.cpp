// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Launch.h"
#include "EngineLoop.h"
#include "GameEngine.h"
#include "Global.h"
#include "GenericPlatform/GenericSplash.h"
#include "GenericPlatform/GenericApplication.h"
#include "Localizational/Name.h"

#if WITH_EDITOR
#include "CoreEd.h"
#endif

NLaunch* NLaunch::CurrentLaunch;

NLaunch::NLaunch(String CmdArgs)
	: CmdArgs(CmdArgs)
{
    check(CurrentLaunch == nullptr);
    CurrentLaunch = this;

    GenericApp = NGenericApplication::CreateApplication();
    Loop = std::make_unique<NEngineLoop>();
}

NLaunch::~NLaunch() noexcept
{
    check(CurrentLaunch);
    CurrentLaunch = nullptr;
}

int32 NLaunch::GuardedMain()
{
    GenericApp->SetApplicationPointer(GetApplicationPointer());
    Global::SetEngineDirectory(GenericApp->GetEngineDirectory());
    Global::SetGameDirectory(Environment::GetCurrentDirectory());

    Loop->PreInit(CmdArgs);
#if WITH_EDITOR
    NCoreEd::EditorInit(*Loop);
#else
    Loop->Init(std::make_shared<GameEngine>());
#endif
    Loop->PostInit();

    std::vector<NGenericPlatformInputEvent> InputEvents;
    while (!GenericApp->IsQuitRequested())
    {
        GenericApp->PumpMessages(InputEvents);
        Loop->Tick(InputEvents);
    }

    Loop->Shutdown();
    return GenericApp->GetExitCode();
}

NLaunch& NLaunch::Get() noexcept
{
    return *CurrentLaunch;
}

extern "C" LAUNCH_API int32 Ayla__Launch__StartApplication(const char_t* const* args, int32 length)
{
	std::vector<String> Args;
	for (int i = 0; i < length; ++i)
	{
		Args.emplace_back(String::FromLiteral(args[i]));
	}

	return NLaunch::GeneratePlatformLaunch(String::Join(TEXT(" "), Args))->GuardedMain();
}