// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Launch.h"
#include "Engine.h"
#include "GenericApplication.h"
#include "Platform/DynamicLibrary.h"

namespace Ayla
{
    Launch::Launch()
    {
    }

    Launch::~Launch() noexcept
    {
    }

    int32 Launch::StartApplication()
    {
        m_Engine = New<Engine>();
        m_Engine->PreInitialize();
        m_Engine->Initialize();

        auto& app = GenericApplication::Get();
        std::vector<GenericPlatformInputEvent> inputEvents;
        while (true)
        {
            app.PumpMessages(inputEvents);
            if (app.IsQuitRequested())
            {
                break;
			}
            m_Engine->Tick();
        }

        m_Engine->Shutdown();
        return app.GetExitCode();
    }

    int32 Launch::GuardedMain(std::vector<String> args, DynamicLibrary& api)
    {
        return try_([&]()
        {
            ThreadPool::Initialize();

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
            auto launch = New<Launch>();
            return launch->StartApplication();
        })
        .finally_([]()
        {
            GC::Collect();
            GC::WaitForCompleteToFinalize();
            
            ThreadPool::Shutdown();
        });
    }
}
