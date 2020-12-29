// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "SceneRenderer.h"

namespace SC::Runtime::Game::SceneRendering
{
	class GAME_API DeferredSceneRenderer : public SceneRenderer
	{
	public:
		using Super = SceneRenderer;
		using This = SceneRenderer;

	private:

	public:
		DeferredSceneRenderer(RHI::IRHIDeviceBundle* deviceBundle);
		~DeferredSceneRenderer() override;

		void BeginRender() override;
		void EndRender() override;
	};
}

#ifdef __SC_GLOBAL_NAMESPACE__
using SC::Runtime::Game::SceneRendering::DeferredSceneRenderer;
#endif