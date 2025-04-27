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
        m_GenericApp = GenericApplication::CreateApplication();
    }

    Launch::~Launch() noexcept
    {
    }

    int32 Launch::StartApplication()
    {
        m_GenericApp->SetApplicationPointer(GetApplicationPointer());
        
#if WITH_EDITOR
        m_Engine = New<EditorEngine>();
#else
        m_Engine = New<::Ayla::Engine>();
#endif

        auto initializationContext = m_Engine->PreInitialize();
        m_Engine->Initialize(initializationContext, CreatePlatformRenderFeature(), m_GenericApp);

        std::vector<GenericPlatformInputEvent> inputEvents;
        while (!m_GenericApp->IsQuitRequested())
        {
            m_GenericApp->PumpMessages(inputEvents);
            m_Engine->Tick();
        }

        return m_GenericApp->GetExitCode();
    }

    RPtr<GenericApplication> Launch::GetApplication()
    {
        return m_GenericApp;
    }
}
