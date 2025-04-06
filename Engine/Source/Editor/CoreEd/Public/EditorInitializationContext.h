// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "InitializationContext.h"
#include "EditorInitializationContext.gen.h"

namespace Ayla
{
	ACLASS()
	class COREED_API EditorInitializationContext : public InitializationContext
	{
		GENERATED_BODY()

	public:
		virtual void Progress(String text) override;
	};
}