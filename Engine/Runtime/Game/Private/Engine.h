// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "IEngineTick.h"

namespace SC::Runtime::Game
{
	namespace RHI
	{
		interface IRHIBundle;
		interface IRHICommandFence;
		interface IRHIDeviceBundle;
		interface IRHIImmediateCommandList;
	}

	class Engine : virtual public Core::Object, virtual public IEngineTick
	{
	public:
		using Super = Core::Object;
		using This = Engine;

	private:
		std::vector<Core::TRefPtr<RHI::IRHIBundle>> rhiBundles;
		Core::TRefPtr<RHI::IRHICommandFence> autoFence;

		RHI::IRHIDeviceBundle* deviceBundle;
		RHI::IRHIImmediateCommandList* immediateCommandList;

		bool bPresent : 1;

	public:
		Engine();
		~Engine() override;

		virtual void Initialize();
		virtual void Tick();

	private:
		void ForEachBundles(std::function<void(RHI::IRHIBundle*)> action);
	};
}