// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/Behavior.h"
#include "Camera.gen.h"

namespace Ayla
{
	ACLASS()
	class ENGINE_API Camera : public Behavior
	{
		GENERATED_BODY()

	public:
		Camera();
		virtual ~Camera() noexcept override;
	};
}