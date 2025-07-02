// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Platform/PlatformMacros.h"

#if PLATFORM_LINUX

#define __ALLOW_PLATFORM_COMMON_H__

#include "Platform/Linux/LinuxPlatformProcess.h"
#include "Platform/PlatformCommon.h"
#include "Console.h"
#include "Path.h"
#include "Diagnostics/StackFrame.h"
#include "Diagnostics/StackTrace.h"
#include "Diagnostics/Debug.h"
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <cstring>
#include <fstream>
#include <dlfcn.h>
#include <execinfo.h>
#include <cxxabi.h>

namespace Ayla
{
    bool LinuxPlatformProcess::IsDebuggerPresent() noexcept
    {
        std::ifstream status("/proc/self/status");
        std::string line;
        while (std::getline(status, line))
        {
            if (line.find("TracerPid:") == 0)
            {
                int tracerPid = atoi(line.substr(10).c_str());
                return tracerPid != 0;
            }
        }
        return false;
    }

    void LinuxPlatformProcess::OutputDebugString(String InMessage) noexcept
    {
        Console::WriteLine(InMessage);
    }

    void LinuxPlatformProcess::StacktraceCurrent(std::vector<StackFrame>& OutStackframes) noexcept
    {
        constexpr int MaxFrames = 64;
        void* frames[MaxFrames];
        int frameCount = backtrace(frames, MaxFrames);

        for (int i = 0; i < frameCount; ++i) {
            Dl_info info;
            String module, description;
            int64 addressOf = reinterpret_cast<int64>(frames[i]);
            if (dladdr(frames[i], &info))
            {
                module = String::FromLiteral(info.dli_fname ? info.dli_fname : "");
                if (info.dli_sname)
                {
                    int status = 0;
                    char* demangled = abi::__cxa_demangle(info.dli_sname, nullptr, nullptr, &status);
                    if (demangled && status == 0)
                    {
                        description = String::FromLiteral(demangled);
                        free(demangled);
                    }
                    else
                    {
                        description = String::FromLiteral(info.dli_sname);
                    }
                }
            }

            OutStackframes.emplace_back(StackFrame
            {
                .Module = module,
                .Description = description
            });
        }
    }

    void LinuxPlatformProcess::StacktraceFromThread(void* Handle, std::vector<StackFrame>& OutStackframes) noexcept
    {
        // 리눅스에서는 안전하게 타 스레드의 스택트레이스를 얻기 어렵기 때문에, 현재 스레드만 지원
        if ((pid_t)(intptr_t)Handle == gettid())
        {
            StacktraceCurrent(OutStackframes);
        }
        else
        {
            OutStackframes.clear();
        }
    }

    void LinuxPlatformProcess::StacktraceFromException(void* /*lpExceptionPointer*/, std::vector<StackFrame>& OutStackframes) noexcept
    {
        // 리눅스에서는 예외 포인터 기반 스택트레이스는 미지원
        StacktraceCurrent(OutStackframes);
    }

    void LinuxPlatformProcess::SetupStacktraceSignals() noexcept
    {
        // SIGSEGV, SIGABRT 등 치명적 시그널 발생 시 스택트레이스를 출력하는 핸들러 등록
        auto handler = [](int sig)
        {
            Console::Error.WriteLine(String::Format(TEXT("Caught signal: {0}\n{1}"), sig, StackTrace::Current().ToString()));
            _exit(128 + sig);
        };

        struct sigaction sa;
        memset(&sa, 0, sizeof(sa));
        sa.sa_handler = handler;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = SA_RESETHAND;

        sigaction(SIGSEGV, &sa, nullptr);
        sigaction(SIGABRT, &sa, nullptr);
        sigaction(SIGFPE,  &sa, nullptr);
        sigaction(SIGILL,  &sa, nullptr);
        sigaction(SIGBUS,  &sa, nullptr);
    }

    String LinuxPlatformProcess::FindEngineDirectory()
    {
        char buf[1024] = {};
        ssize_t len = readlink("/proc/self/exe", buf, sizeof(buf) - 1);
        if (len <= 0)
            return String::GetEmpty();

        String exePath = String::FromLiteral(std::string_view(buf, (size_t)len));
        String binDir = Path::GetDirectoryName(exePath);
        String engineDir = Path::Combine(binDir, TEXT(".."), TEXT(".."), TEXT(".."));
        engineDir = Path::GetFullPath(engineDir);
        return engineDir;
    }

    void* LinuxPlatformProcess::AllocateCurrentThreadHandle() noexcept
    {
        return (void*)(intptr_t)gettid();
    }

    void LinuxPlatformProcess::DestroyCurrentThreadHandle(void* /*InHandle*/) noexcept
    {
        // Nothing to do on Linux
    }

    void LinuxPlatformProcess::SetThreadDescription(void* InHandle, String InDescription) noexcept
    {
        // InHandle이 nullptr이면 현재 스레드에 적용
        pthread_t thread;
        if (InHandle == nullptr)
        {
            thread = pthread_self();
        }
        else
        {
            // gettid()로 얻은 tid를 pthread_t로 변환 (플랫폼에 따라 다를 수 있음)
            // 대부분의 리눅스에서는 pthread_t와 tid가 다르므로, 안전하게는 현재 스레드만 지원
            thread = pthread_self();
        }

        // pthread_setname_np는 16자 제한이 있음
        if (InDescription.length() > 15)
        {
            InDescription = InDescription.Substring(0, 15);
        }

        pthread_setname_np(thread, InDescription.AsCodepage().c_str());
    }

    void* LinuxPlatformProcess::CreateProcess(const ProcessStartInfo& InStartInfo) noexcept
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            // Child process
            std::vector<char*> args;
            args.push_back(const_cast<char*>(InStartInfo.FileName.AsCodepage().c_str()));
            std::string argStr = InStartInfo.Arguments.string();
            std::istringstream iss(argStr);
            std::string token;
            while (iss >> token)
            {
                args.push_back(strdup(token.c_str()));
            }
            args.push_back(nullptr);

            execvp(InStartInfo.FileName.AsCodepage().c_str(), args.data());
            _exit(127);
        }
        else if (pid > 0)
        {
            return (void*)(intptr_t)pid;
        }
        else
        {
            return nullptr;
        }
    }

    void LinuxPlatformProcess::CloseProcessHandle(void* InHandle) noexcept
    {
        pid_t pid = (pid_t)(intptr_t)InHandle;
        int status = 0;
        waitpid(pid, &status, 0);
    }

    bool LinuxPlatformProcess::SetEnvironmentVariable(String InName, String InValue) noexcept
    {
        return setenv(InName.AsCodepage().c_str(), InValue.AsCodepage().c_str(), 1) == 0;
    }

    String LinuxPlatformProcess::GetEnvironmentVariable(String InName) noexcept
    {
        const char* val = getenv(InName.AsCodepage().c_str());
        if (!val)
            return String::GetEmpty();
        return String::FromLiteral(val);
    }

	void* LinuxPlatformProcess::LoadLibrary(String fileName) noexcept
	{
		auto handle = dlopen(fileName.AsCodepage().c_str(), RTLD_NOW | RTLD_GLOBAL);
        if (handle == nullptr)
        {
            auto err = dlerror();
            auto s = String::FromCodepage(err);
            OutputDebugString(s);
        }
        return handle;
	}

	void LinuxPlatformProcess::FreeLibrary(void* handle) noexcept
	{
		dlclose(handle);
	}

	void* LinuxPlatformProcess::GetLibraryFunction(void* handle, String functionName) noexcept
	{
		return dlsym(handle, functionName.AsCodepage().c_str());
	}
}

#undef __ALLOW_PLATFORM_COMMON_H__

#endif