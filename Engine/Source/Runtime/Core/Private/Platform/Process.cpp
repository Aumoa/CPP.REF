// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Platform/Process.h"
#include "Platform/PlatformProcess.h"

Process::Process()
{
}

Process::~Process() noexcept
{
	if (ProcessHandle)
	{
		PlatformProcess::CloseProcessHandle(ProcessHandle);
		ProcessHandle = nullptr;
	}
}

std::shared_ptr<Process> Process::Start(const ProcessStartInfo& InStartInfo)
{
	std::shared_ptr<Process> Ptr{ new Process() };
	Ptr->StartInfo = InStartInfo;
	Ptr->StartApplication();
	return Ptr;
}

void Process::StartApplication()
{
	ProcessHandle = PlatformProcess::CreateProcess(StartInfo);
}