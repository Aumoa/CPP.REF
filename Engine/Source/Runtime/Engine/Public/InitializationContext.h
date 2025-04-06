// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "InitializationContext.gen.h"

namespace Ayla
{
	ACLASS()
	class ENGINE_API InitializationContext : public Object
	{
		GENERATED_BODY()

	public:
		virtual void Progress(String text);
	};
}