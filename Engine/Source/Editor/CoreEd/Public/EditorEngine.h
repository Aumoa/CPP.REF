// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "EditorEngine.gen.h"

namespace Ayla
{
	ACLASS()
	class COREED_API EditorEngine : public Engine
	{
		GENERATED_BODY()

	public:
		EditorEngine();

		virtual RPtr<InitializationContext> PreInitialize() override;
		virtual void Initialize(RPtr<InitializationContext> context) override;
	};
}