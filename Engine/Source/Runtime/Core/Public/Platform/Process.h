// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/ProcessStartInfo.h"

class CORE_API Process
{
private:
	ProcessStartInfo StartInfo;
	void* ProcessHandle = nullptr;

private:
	Process();

public:
	~Process() noexcept;

	const ProcessStartInfo& GetStartInfo() const { return StartInfo; }

	static std::shared_ptr<Process> Start(const ProcessStartInfo& InStartInfo);

private:
	void StartApplication();
};