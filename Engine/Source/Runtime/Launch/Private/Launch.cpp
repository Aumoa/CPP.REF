// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Launch.h"
#include "Engine.h"
#include "GenericApplication.h"
#include "CommandLineParser.h"
#include "LaunchOptions.h"
#include "Platform/DynamicLibrary.h"
#include "ScriptingBackend/CoreCLR/CoreCLRScriptingBackend.h"
#include "IO/IOCompletionOverlapped.h"
#include "Environment.h"
#include <fstream>

namespace Ayla
{
    namespace
    {
        bool ShouldTraceLaunch() noexcept
        {
            try
            {
                return !Environment::GetEnvironmentVariable(TEXT("AYLA_LAUNCH_TRACE")).IsEmpty();
            }
            catch (...)
            {
                return false;
            }
        }

        void WriteUnhandledExceptionLog(const String& gameAssembly, const String& details) noexcept
        {
            try
            {
                auto logPath = Path::Combine(Path::GetDirectoryName(gameAssembly), TEXT("Launch.error.log"));
                std::ofstream stream(logPath.AsCodepage(), std::ios::binary | std::ios::trunc);
                stream << details.AsCodepage();
            }
            catch (...)
            {
            }
        }

        void WriteLaunchTrace(const String& gameAssembly, const String& phase) noexcept
        {
            if (!ShouldTraceLaunch())
            {
                return;
            }

            try
            {
                auto logPath = Path::Combine(Path::GetDirectoryName(gameAssembly), TEXT("Launch.trace.log"));
                std::ofstream stream(logPath.AsCodepage(), std::ios::binary | std::ios::app);
                stream << phase.AsCodepage() << "\r\n";
            }
            catch (...)
            {
            }
        }
    }

    Launch::Launch()
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

        String gameAssembly = it->second[0].value();
        try
        {
            WriteLaunchTrace(gameAssembly, TEXT("Creating CoreCLR scripting backend."));
            auto scriptingBackend = std::make_unique<CoreCLRScriptingBackend>();

            WriteLaunchTrace(gameAssembly, TEXT("Loading game assembly."));
            scriptingBackend->LoadAssembly(Path::GetDirectoryName(gameAssembly), Path::GetFileNameWithoutExtension(gameAssembly));

            WriteLaunchTrace(gameAssembly, TEXT("Entering guarded main."));
            return try__
            {
                auto launch = New<Launch>();
                try
                {
                    auto launchOptions = New<LaunchOptions>(platform, std::move(args));
                    return launch->GuardedMain(launchOptions);
                }
                catch (const Exception& ex)
                {
                    WriteUnhandledExceptionLog(gameAssembly, ex.ToString());
                    throw;
                }
                catch (const std::exception& ex)
                {
                    WriteUnhandledExceptionLog(gameAssembly, String::FromCodepage(ex.what()));
                    throw;
                }
                catch (...)
                {
                    WriteUnhandledExceptionLog(gameAssembly, TEXT("Unknown native exception."));
                    throw;
                }
            }
            finally__
            {
                IOCompletionOverlapped::Assert();
            }
            end_try__;
        }
        catch (const Exception& ex)
        {
            WriteUnhandledExceptionLog(gameAssembly, ex.ToString());
            throw;
        }
        catch (const std::exception& ex)
        {
            WriteUnhandledExceptionLog(gameAssembly, String::FromCodepage(ex.what()));
            throw;
        }
        catch (...)
        {
            WriteUnhandledExceptionLog(gameAssembly, TEXT("Unknown native exception."));
            throw;
        }
	}
}
