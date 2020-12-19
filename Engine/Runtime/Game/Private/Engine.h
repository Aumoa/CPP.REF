// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "IEngineTick.h"

#include "Logging/LogCategoryBase.h"

namespace SC::Runtime::Game
{
	namespace RHI
	{
		interface IRHIBundle;
		interface IRHICommandFence;
		interface IRHIDeviceBundle;
		interface IRHIImmediateCommandList;
		interface IRHISwapChain;
	}

	class Engine : virtual public Core::Object, virtual public IEngineTick
	{
	public:
		using Super = Core::Object;
		using This = Engine;

	private:
		Logging::LogCategoryBase LogEngine;

		std::vector<Core::TRefPtr<RHI::IRHIBundle>> rhiBundles;
		Core::TRefPtr<RHI::IRHICommandFence> autoFence;

		RHI::IRHIDeviceBundle* deviceBundle;
		RHI::IRHIImmediateCommandList* immediateCommandList;
		RHI::IRHISwapChain* swapChain;

		bool bPresent : 1;

	public:
		Engine();
		~Engine() override;

		virtual void Initialize();
		virtual void Tick();

		vs_property_get(RHI::IRHIDeviceBundle*, DeviceBundle);
		RHI::IRHIDeviceBundle* DeviceBundle_get() const;

		static Engine* gEngine;

	private:
		void ForEachBundles(std::function<void(RHI::IRHIBundle*)> action);
	};
}

#define GEngine (*SC::Runtime::Game::Engine::gEngine)