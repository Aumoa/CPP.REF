// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "PlatformMisc/PlatformApplication.h"
#include "NetworkApplication.gen.h"

SCLASS()
class NETWORK_API NetworkApplication : public PlatformApplication
{
	GENERATED_BODY()

protected:
	std::chrono::milliseconds TickInterval;

private:
	std::chrono::steady_clock::time_point _intervalNext;

public:
	NetworkApplication();

	virtual int32 Startup(const CommandLineBuilder& args) override;
	virtual void Tick() override;

protected:
	virtual void Start();
	virtual void Update(const TimeSpan& deltaTime);

protected:
	virtual void OnApplicationSignalExit() noexcept override;
	virtual void OnApplicationShutdown() noexcept override;

public:
	static NetworkApplication* GetApp();
};