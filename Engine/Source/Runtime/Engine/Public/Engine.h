// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine.gen.h"

namespace Ayla
{
	class InitializationContext;

	ACLASS()
	class ENGINE_API Engine : public Object
	{
		GENERATED_BODY()

	public:
		Engine();

		virtual RPtr<InitializationContext> PreInitialize();
		virtual void Initialize(RPtr<InitializationContext> context);
	};
}