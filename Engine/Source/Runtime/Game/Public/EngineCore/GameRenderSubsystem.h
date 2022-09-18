// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "EngineSubsystem.h"
#include "GameRenderSubsystem.generated.h"

class RHIRenderThread;
class RHIDevice;
class RHICommandQueue;
class RHIFence;
class SceneView;

SCLASS()
class GAME_API GameRenderSubsystem : public EngineSubsystem
{
	GENERATED_BODY()

private:
	static GameRenderSubsystem* sInstance;

private:
	SPROPERTY()
	RHIRenderThread* RenderThread = nullptr;

	std::shared_ptr<RHIDevice> Device;
	std::shared_ptr<RHICommandQueue> CommandQueue;
	std::shared_ptr<RHIFence> Fence;
	uint64 FenceValue = 0;

	Task<> PreviousRenderTask;
	std::vector<SceneView*> Scenes;

public:
	GameRenderSubsystem();
	virtual ~GameRenderSubsystem() noexcept override;

	virtual void Init();
	virtual void Deinit();

	void RegisterSceneView(SceneView* Scene);
	void ExecuteRenderTicks(std::function<void()> DisplayWorks);
	void JoinRenderThread();

	inline const std::shared_ptr<RHIDevice>& GetDevice() { return Device; }
	inline const std::shared_ptr<RHICommandQueue>& GetCommandQueue() { return CommandQueue; }

public:
	static bool IsInRenderThread() noexcept;
};