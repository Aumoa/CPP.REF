// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/Behavior.h"
#include "ScriptableBehavior.gen.h"

namespace Ayla
{
	ACLASS()
	class ENGINE_API ScriptableBehavior : public Behavior
	{
		GENERATED_BODY()

	public:
		ScriptableBehavior();
		virtual ~ScriptableBehavior() noexcept override;
	};
}