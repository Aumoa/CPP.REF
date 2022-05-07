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
		SPROPERTY(RenderContext)
		SRenderContext* RenderContext = nullptr;

	public:
		SGameRenderSystem();

		SFUNCTION(Inject)
		void Inject(SRenderEngine* RenderEngine);

		virtual Task<> StartAsync(std::stop_token CancellationToken) override;
		virtual Task<> StopAsync(std::stop_token CancellationToken) override;

		void RenderFrame(std::chrono::duration<float> InDeltaTime);
	};
}