// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

namespace SC::Runtime::Game::RHI
{
	interface IRHIResource;
	interface IRHIDeferredCommandList;
	interface IRHIDeviceBundle;
}

namespace SC::Runtime::Game::SceneRendering
{
	class GAME_API SceneRenderer : virtual public Core::Object
	{
	public:
		using Super = Core::Object;
		using This = SceneRenderer;

	private:
		RHI::IRHIDeviceBundle* deviceBundle;
		Core::TRefPtr<RHI::IRHIResource> finalColor;
		Core::TRefPtr<RHI::IRHIDeferredCommandList> commandList;

	public:
		SceneRenderer(RHI::IRHIDeviceBundle* deviceBundle);
		~SceneRenderer() override;

		void BeginRender();
		void EndRender();

	private:
		// CALLBACK HANDLERS
		void Application_OnPostSized(int32 width, int32 height);
	};
}