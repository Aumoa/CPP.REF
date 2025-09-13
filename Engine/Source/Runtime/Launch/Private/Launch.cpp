// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Launch.h"
#include "Engine.h"
#include "GenericApplication.h"
#include "CommandLineParser.h"
#include "Platform/DynamicLibrary.h"
#include "ScriptingBackend/CoreCLR/CoreCLRScriptingBackend.h"

namespace Ayla
{
    Launch::Launch(std::unique_ptr<CommandLineParser> args) : m_Args{ std::move(args) }
    {
    }

    Launch::~Launch() noexcept
    {
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

        auto& options = args->Options();
        auto it = options.find(TEXT("gameassembly"));
        if (it == options.end())
        {
            throw InvalidOperationException(TEXT("No game assembly specified."));
        }

        auto gameAssembly = it->second[0].value();
        auto scriptingBackend = std::make_unique<CoreCLRScriptingBackend>();
        scriptingBackend->LoadAssembly(Path::GetDirectoryName(gameAssembly), Path::GetFileNameWithoutExtension(gameAssembly));

        return try__
        {
            auto app = std::unique_ptr<GenericApplication>(loader());
			New<Launch>(std::move(args))->StartApplication();
            return GenericApplication::Get().GetExitCode();
        }
        catch (const Exception& e)
        {
            Debug::LogCriticalFormat(TEXT("LogException"), e.GetMessage());
            return -1;
        }
        finally__
        {
            ThreadPool::Shutdown();
        }
        end_try__;
	}
}
