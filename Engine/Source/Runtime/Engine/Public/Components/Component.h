// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SerializableObject.h"
#include "Component.gen.h"

namespace Ayla
{
	ACLASS()
	class ENGINE_API Component : public SerializableObject
	{
		GENERATED_BODY()

	public:
		Component();
		virtual ~Component() noexcept override;
	};
}