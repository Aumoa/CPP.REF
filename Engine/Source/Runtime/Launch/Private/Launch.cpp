// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Launch.h"
#include "GenericPlatform/GenericApplication.h"
#include "GenericPlatform/GenericWindow.h"
#include "Localizational/Name.h"
#include "Platform/DynamicLibrary.h"

namespace Ayla
{
    Launch::Launch(GenericApplication* genericApp)
        : m_GenericApp{ genericApp }
    {
    }

    Launch::~Launch() noexcept
    {
    }

    int32 Launch::StartApplication(void* applicationPointer)
    {
        m_GenericApp->SetApplicationPointer(applicationPointer);

        GenericWindowDefinition wDef =
        {
            .bPrimaryWindow = true
        };
        auto window = m_GenericApp->MakeWindow(wDef);
        window->Show();
        
// #if WITH_EDITOR
//         m_Engine = New<EditorEngine>();
// #else
//         m_Engine = New<::Ayla::Engine>();
// #endif

//         auto initializationContext = m_Engine->PreInitialize();
//         m_Engine->Initialize(initializationContext, CreatePlatformRenderFeature(), m_GenericApp);

        std::vector<GenericPlatformInputEvent> inputEvents;
        while (!m_GenericApp->IsQuitRequested())
        {
            m_GenericApp->PumpMessages(inputEvents);
            //m_Engine->Tick(inputEvents);
        }

        return m_GenericApp->GetExitCode();
    }

    GenericApplication* Launch::GetApplication()
    {
        return m_GenericApp;
    }

    int32 Launch::GuardedMain(std::vector<String> args, DynamicLibrary& api, void* applicationPointer)
    {
        return try_([&]()
        {
            if (api.IsValid() == false)
            {
                throw InvalidOperationException(TEXT("Failed to load Platform API set."));
            }

            auto loader = api.LoadFunction<GenericApplication*>(NAMEOF_CREATE_GENERIC_APPLICATION);
            if (loader == nullptr)
            {
                throw InvalidOperationException(TEXT("Failed to load signature for create generic application."));
            }

            auto app = std::unique_ptr<GenericApplication>{ loader() };
            auto launch = New<Launch>(app.get());
            return launch->StartApplication(applicationPointer);
        })
        .finally_([]()
        {
            GC::Collect();
            GC::WaitForCompleteToFinalize();
        });
    }
}
