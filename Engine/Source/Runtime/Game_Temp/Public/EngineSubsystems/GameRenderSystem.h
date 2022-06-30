// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "GameEngineSubsystem.h"

namespace libty::inline Game
{
	class GAME_API SGameRenderSystem : public SGameEngineSubsystem
	{
		GENERATED_BODY(SGameRenderSystem);

	private:
		SPROPERTY(RenderEngine)
		SRenderEngine* RenderEngine = nullptr;
		SPROPERTY(RenderThread)
		SRenderThread* RenderThread = nullptr;
		SPROPERTY(SlateApp)
		SSlateApplication* SlateApp = nullptr;

		SPROPERTY(RenderContext)
		SRenderContext* RenderContext = nullptr;

		SPROPERTY(Fence)
		IRHIFence* Fence = nullptr;
		uint64 FenceValue = 0;

	public:
		SGameRenderSystem();

		SFUNCTION(Inject)
		void Inject(SRenderEngine* RenderEngine, SRenderThread* RenderThread, SSlateApplication* SlateApp);

		virtual Task<> StartAsync(std::stop_token CancellationToken) override;
		virtual Task<> StopAsync(std::stop_token CancellationToken) override;

		void RenderFrame(std::chrono::duration<float> InDeltaTime);
	};
}