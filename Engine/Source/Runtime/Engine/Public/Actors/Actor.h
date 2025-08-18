// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SerializableObject.h"
#include "Actor.gen.h"

namespace Ayla
{
	ACLASS()
	class ENGINE_API Actor : public SerializableObject
	{
		GENERATED_BODY()

	public:
		Actor();
		virtual ~Actor() noexcept override;
	};
}