// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

namespace SC::Runtime::Game::RHI
{
	interface IRHIResource;
	interface IRHIDeferredCommandList;
	interface IRHIDeviceBundle;
	interface IRHIRenderTargetView;
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
		Core::TRefPtr<RHI::IRHIRenderTargetView> rtv;

	public:
		SceneRenderer(RHI::IRHIDeviceBundle* deviceBundle);
		~SceneRenderer() override;

		void BeginRender();
		void EndRender();

		vs_property_get(RHI::IRHIDeferredCommandList*, CommandList);
		RHI::IRHIDeferredCommandList* CommandList_get() const;
		vs_property_get(RHI::IRHIResource*, FinalColor);
		RHI::IRHIResource* FinalColor_get() const;

	private:
		// CALLBACK HANDLERS
		void Application_OnPostSized(int32 width, int32 height);
	};
}