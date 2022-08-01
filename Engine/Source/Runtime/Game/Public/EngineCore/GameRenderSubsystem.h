// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "EngineSubsystem.h"
#include "GameRenderSubsystem.generated.h"

class RHIRenderThread;
class RHIDevice;
class RHICommandQueue;
class RHIFence;
class RHICommandList;

SCLASS()
class GAME_API GameRenderSubsystem : public EngineSubsystem
{
	GENERATED_BODY()

private:
	SPROPERTY()
	RHIRenderThread* _renderThread = nullptr;

	std::shared_ptr<RHIDevice> _device;
	std::shared_ptr<RHICommandQueue> _commandQueue;
	std::shared_ptr<RHIFence> _fence;
	std::shared_ptr<RHICommandList> _commandList;
	uint64 _fenceValue = 0;

	Task<> _previousRenderTick;

public:
	GameRenderSubsystem();
	virtual ~GameRenderSubsystem() noexcept override;

	virtual void Init();
	virtual void Deinit();

	void ExecuteRenderTicks(std::function<void()> presentWorks);
	void JoinRenderThread();

	inline const std::shared_ptr<RHIDevice>& GetDevice() { return _device; }
	inline const std::shared_ptr<RHICommandQueue>& GetCommandQueue() { return _commandQueue; }
};