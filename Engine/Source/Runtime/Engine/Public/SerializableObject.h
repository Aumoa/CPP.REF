// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SerializableObject.gen.h"

namespace Ayla
{
	ACLASS()
	class ENGINE_API SerializableObject : public Object
	{
		GENERATED_BODY()

	public:
		SerializableObject();
		virtual ~SerializableObject() noexcept override;
	};
}