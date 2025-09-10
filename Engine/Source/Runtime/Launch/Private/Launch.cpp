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

    int32 Launch::StartApplication()
    {
        auto& options = m_Args->Options();
        
        auto it = options.find(TEXT("gameassembly"));
        if (it == options.end())
        {
			throw InvalidOperationException(TEXT("No game assembly specified."));
        }

        auto gameAssembly = it->second[0].value();
        m_ScriptingBackend = std::make_unique<CoreCLRScriptingBackend>();
        m_ScriptingBackend->LoadAssembly(Path::GetDirectoryName(gameAssembly), Path::GetFileNameWithoutExtension(gameAssembly));
        
        using CreateByNativeDelegate = ObjectReferenceWrapper(*)(const wchar_t*);
        auto* createByNative = reinterpret_cast<CreateByNativeDelegate>(m_ScriptingBackend->GetFunctionPointer(TEXT("Engine.Script"), TEXT("Ayla.Engine"), TEXT("CreateByNative")));
        m_Engine = createByNative(gameAssembly.c_str()).Resolve<Engine>();
        m_Engine->GuardedStartup();
        m_Engine->GuardedLoop();
        return GenericApplication::Get().GetExitCode();
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
