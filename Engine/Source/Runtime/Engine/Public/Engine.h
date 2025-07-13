// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine.gen.h"

namespace Ayla
{
	class Graphics;
	class GenericActivity;
	class GenericWindowSwapchainExtension;

	ACLASS()
	class ENGINE_API Engine : public Object
	{
		GENERATED_BODY()

	private:
		std::shared_ptr<GenericActivity> m_MainActivity;
		std::shared_ptr<Graphics> m_Graphics;
		std::vector<std::shared_ptr<GenericWindowSwapchainExtension>> m_SwapchainExtensions;

	public:
		Engine();
		virtual ~Engine() noexcept override;

		virtual void PreInitialize();
		virtual void Initialize();

		virtual void Tick();
	};
}