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
        auto& app = GenericApplication::Get();

        m_Engine = Object::New<Engine>();
        m_Engine->PreInitialize();
        m_Engine->Initialize(m_Args.get());

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

    int32 Launch::GuardedMain(std::unique_ptr<CommandLineParser> args, const DynamicLibrary& apiSet)
    {
        ThreadPool::Initialize();

        if (apiSet.IsValid() == false)
        {
            throw InvalidOperationException(TEXT("Failed to load Platform API set."));
        }

        auto loader = apiSet.LoadFunction<GenericApplication*>(NAMEOF_CREATE_GENERIC_APPLICATION);
        if (loader == nullptr)
        {
            throw InvalidOperationException(TEXT("Failed to load signature for create generic application."));
        }

        return try__
        {
            auto app = std::unique_ptr<GenericApplication>(loader());
			return std::make_unique<Launch>(std::move(args))->StartApplication();
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
}
