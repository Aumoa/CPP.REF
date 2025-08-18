// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SerializableObject.h"
#include "Scene.gen.h"

namespace Ayla
{
	ACLASS()
	class ENGINE_API Scene : public SerializableObject
	{
		GENERATED_BODY()

	public:
		Scene();
		virtual ~Scene() noexcept override;
	};
}