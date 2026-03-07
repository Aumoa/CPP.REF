// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Rendering/InputElementDescription.h"
#include "VertexFactory.gen.h"

namespace Ayla
{
	ACLASS()
	class RENDERCORE_API VertexFactory : public Object
	{
		GENERATED_BODY()

	public:
		virtual std::vector<InputElementDescription> GetInputElements() const = 0;
	};
}