// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Launch.h"
#include "Engine.h"
#include "GenericApplication.h"
#include "CommandLineParser.h"
#include "Platform/DynamicLibrary.h"
#include "ScriptingBackend/CoreCLR/CoreCLRScriptingBackend.h"

namespace Ayla
{
    Launch::Launch(std::unique_ptr<CommandLineParser> args)
        : m_Args(std::move(args))
    {
    }

    Launch::~Launch() noexcept
    {
    }

    int32 Launch::Main(std::unique_ptr<CommandLineParser> args, String platform)
    {
        auto& options = args->Options();
        auto it = options.find(TEXT("gameassembly"));
        if (it == options.end())
        {
            throw InvalidOperationException(TEXT("No game assembly specified."));
        }

        auto gameAssembly = it->second[0].value();
        auto scriptingBackend = std::make_unique<CoreCLRScriptingBackend>();
        scriptingBackend->LoadAssembly(Path::GetDirectoryName(gameAssembly), Path::GetFileNameWithoutExtension(gameAssembly));

        ThreadPool::Initialize();

        auto launch = New<Launch>(std::move(args));
        return launch->GuardedMain(platform);
	}
}
