// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Platform/PlatformMacros.h"

#if PLATFORM_OSX

#define __ALLOW_PLATFORM_COMMON_H__

#include "Platform/OSX/OSXPlatformProcess.h"
#include "Platform/PlatformCommon.h"
#include "Console.h"
#include "Path.h"
#include <sys/types.h>
#include <sys/sysctl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <cstring>
#include <fstream>
#include <dlfcn.h>
#include <mach-o/dyld.h>
#include <pthread.h>

namespace Ayla
{
	bool OSXPlatformProcess::IsDebuggerPresent() noexcept
	{
		int mib[4];
		struct kinfo_proc info;
		size_t size = sizeof(info);

		info.kp_proc.p_flag = 0;
		mib[0] = CTL_KERN;
		mib[1] = KERN_PROC;
		mib[2] = KERN_PROC_PID;
		mib[3] = getpid();

		if (sysctl(mib, 4, &info, &size, NULL, 0) == -1)
		{
			return false;
		}

		return (info.kp_proc.p_flag & P_TRACED) != 0;
	}

	void OSXPlatformProcess::OutputDebugString(String InMessage) noexcept
	{
		Console::Write(InMessage);
	}

	String OSXPlatformProcess::FindEngineDirectory()
	{
		char buf[1024] = {};
		uint32_t bufSize = sizeof(buf);
		if (_NSGetExecutablePath(buf, &bufSize) != 0)
			return String::GetEmpty();

		char realBuf[1024] = {};
		if (realpath(buf, realBuf) == nullptr)
			return String::GetEmpty();

		String exePath = String::FromLiteral(realBuf);
		String binDir = Path::GetDirectoryName(exePath);
		String engineDir = Path::Combine(binDir, TEXT(".."), TEXT(".."), TEXT(".."));
		engineDir = Path::GetFullPath(engineDir);
		return engineDir;
	}

	void* OSXPlatformProcess::AllocateCurrentThreadHandle() noexcept
	{
		uint64_t tid = 0;
		pthread_threadid_np(nullptr, &tid);
		return (void*)(intptr_t)tid;
	}

	void OSXPlatformProcess::DestroyCurrentThreadHandle(void*) noexcept
	{
	}

	void OSXPlatformProcess::SetThreadDescription(void* InHandle, String InDescription) noexcept
	{
		if (InDescription.length() > 15)
		{
			InDescription = InDescription.Substring(0, 15);
		}

		pthread_setname_np(InDescription.AsCodepage().c_str());
	}

	void* OSXPlatformProcess::CreateProcess(const ProcessStartInfo& InStartInfo) noexcept
	{
		pid_t pid = fork();
		if (pid == 0)
		{
			std::string fileNameStr = InStartInfo.FileName.AsCodepage();
			std::vector<char*> args;
			args.push_back(const_cast<char*>(fileNameStr.c_str()));
			std::string argStr = InStartInfo.Arguments.string();
			std::istringstream iss(argStr);
			std::string token;
			while (iss >> token)
			{
				args.push_back(strdup(token.c_str()));
			}
			args.push_back(nullptr);

			execvp(fileNameStr.c_str(), args.data());
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

	void OSXPlatformProcess::CloseProcessHandle(void* InHandle) noexcept
	{
		pid_t pid = (pid_t)(intptr_t)InHandle;
		int status = 0;
		waitpid(pid, &status, 0);
	}

	bool OSXPlatformProcess::SetEnvironmentVariable(String InName, String InValue) noexcept
	{
		return setenv(InName.AsCodepage().c_str(), InValue.AsCodepage().c_str(), 1) == 0;
	}

	String OSXPlatformProcess::GetEnvironmentVariable(String InName) noexcept
	{
		const char* val = getenv(InName.AsCodepage().c_str());
		if (!val)
			return String::GetEmpty();
		return String::FromLiteral(val);
	}

	void* OSXPlatformProcess::LoadLibrary(String fileName) noexcept
	{
		auto directory = Path::GetDirectoryName(fileName);
		fileName = String::Format(TEXT("{0}/lib{1}.dylib"), directory, Path::GetFileName(fileName));
		auto handle = dlopen(fileName.AsCodepage().c_str(), RTLD_NOW | RTLD_GLOBAL);
		if (handle == nullptr)
		{
			auto err = dlerror();
			auto s = String::FromCodepage(err);
			OutputDebugString(s);
		}
		return handle;
	}

	void OSXPlatformProcess::FreeLibrary(void* handle) noexcept
	{
		dlclose(handle);
	}

	void* OSXPlatformProcess::GetLibraryFunction(void* handle, String functionName) noexcept
	{
		return dlsym(handle, functionName.AsCodepage().c_str());
	}
}

#undef __ALLOW_PLATFORM_COMMON_H__

#endif
