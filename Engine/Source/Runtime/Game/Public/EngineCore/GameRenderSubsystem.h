// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "EngineSubsystem.h"
#include "GameRenderSubsystem.generated.h"

class RHIRenderThread;
class RHIDevice;
class RHICommandQueue;

SCLASS()
class GAME_API GameRenderSubsystem : public EngineSubsystem
{
	GENERATED_BODY()

private:
	SPROPERTY()
	RHIRenderThread* _renderThread = nullptr;

	std::shared_ptr<RHIDevice> _device;
	std::shared_ptr<RHICommandQueue> _commandQueue;

public:
	GameRenderSubsystem();
	virtual ~GameRenderSubsystem() noexcept override;

	virtual void Init();
	virtual void Deinit();
};