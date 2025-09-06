// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Launch.h"
#include "Engine.h"
#include "GenericApplication.h"
#include "CommandLineParser.h"
#include "Platform/DynamicLibrary.h"

namespace Ayla
{
    Launch::Launch(std::unique_ptr<CommandLineParser> args) : m_Args{ std::move(args) }
    {
    }

    Launch::~Launch() noexcept
    {
    }

    int32 Launch::StartApplication()
    {
#if PLATFORM_WINDOWS
        auto api = DynamicLibrary(TEXT("WindowsAPI"));
#elif PLATFORM_LINUX
        auto api = DynamicLibrary(TEXT("LinuxAPI"));
#else
#error Unsupported platform.
#endif

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

        return try__
        {
            
            m_Engine = New<Engine>();
            m_Engine->PreInitialize();
            m_Engine->Initialize(m_Args.get());

            std::vector<GenericPlatformInputEvent> inputEvents;
            while (true)
            {
                app->PumpMessages(inputEvents);
                if (app->IsQuitRequested())
                {
                    break;
			    }
                m_Engine->Tick();
            }

            m_Engine->Shutdown();
            return app->GetExitCode();
        }
        catch (const Exception& e)
        {
            Debug::LogCriticalFormat(TEXT("LogException"), e.GetMessage());
            return -1;
        }
        finally__
        {
            GC::Collect();
            GC::WaitForCompleteToFinalize();            
            ThreadPool::Shutdown();
        }
        end_try__;
    }

    RPtr<Launch> Launch::CreateInstance(std::vector<String> args)
    {
		return New<Launch>(std::make_unique<CommandLineParser>(args));
    }
}
