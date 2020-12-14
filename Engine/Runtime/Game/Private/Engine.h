// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "IEngineTick.h"

#include "RHI/IRHIBundle.h"

namespace SC::Runtime::Game
{
	class Engine : virtual public Core::Object, virtual public IEngineTick
	{
	public:
		using Super = Core::Object;
		using This = Engine;

	private:
		std::vector<Core::TRefPtr<RHI::IRHIBundle>> rhiBundles;

	public:
		Engine();
		~Engine() override;

		virtual void Initialize();
		virtual void Tick();
	};
}