// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Platform/PlatformMacros.h"

#if PLATFORM_LINUX

#define __ALLOW_PLATFORM_COMMON_H__

#include "Platform/Linux/LinuxPlatformProcess.h"
#include "Platform/PlatformCommon.h"
#include "Console.h"
#include "Path.h"
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
		Console::Write(InMessage);
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

	void LinuxPlatformProcess::DestroyCurrentThreadHandle(void*) noexcept
	{
	}

	void LinuxPlatformProcess::SetThreadDescription(void* InHandle, String InDescription) noexcept
	{
		pthread_t thread;
		if (InHandle == nullptr)
		{
			thread = pthread_self();
		}
		else
		{
			thread = pthread_self();
		}

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
		auto directory = Path::GetDirectoryName(fileName);
		fileName = String::Format(TEXT("{0}/lib{1}.so"), directory, Path::GetFileName(fileName));
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