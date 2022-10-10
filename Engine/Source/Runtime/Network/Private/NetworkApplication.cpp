// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "NetworkApplication.h"
#include "NetworkApplication.gen.cpp"

static constexpr LogCategory LogNet(TEXT("LogNet"));

NetworkApplication::NetworkApplication()
	: Super()
	, TickInterval(33ms)
{
}

int32 NetworkApplication::Startup(const CommandLineBuilder& args)
{
	Log::Info(LogNet, TEXT("Startup application."));

	Start();

	_intervalNext = std::chrono::steady_clock::now();
	StartApplicationLoop();

	return GetPlatformExitCode();
}

void NetworkApplication::Tick()
{
	Super::Tick();

	auto now = std::chrono::steady_clock::now();
	if (now < _intervalNext)
	{
		std::chrono::milliseconds span = std::chrono::duration_cast<std::chrono::milliseconds>(_intervalNext - now);
		if (span > 1ms)
		{
			std::this_thread::sleep_until(_intervalNext);
			return;
		}
		else
		{
			std::this_thread::yield();
			return;
		}
	}

	_intervalNext += TickInterval;
	Update(TickInterval);
}

void NetworkApplication::Start()
{
}

void NetworkApplication::Update(const TimeSpan& deltaTime)
{
}

void NetworkApplication::OnApplicationSignalExit() noexcept
{
	Log::Info(LogNet, TEXT("SIGINT interrupted."));
	Super::OnApplicationSignalExit();
}

void NetworkApplication::OnApplicationShutdown() noexcept
{
	Log::Info(LogNet, TEXT("Application shutting down."));
}

NetworkApplication* NetworkApplication::GetApp()
{
	return Cast<NetworkApplication>(Super::GetApp());
}