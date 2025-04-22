// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Graphics.gen.h"

namespace Ayla
{
	ACLASS()
	class RENDERCORE_API Graphics : public Object
	{
		GENERATED_BODY()

	protected:
		Graphics();

	public:
		virtual void Initialize() = 0;
	};
}