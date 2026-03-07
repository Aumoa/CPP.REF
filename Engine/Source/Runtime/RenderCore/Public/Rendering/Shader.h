// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Shader.gen.h"

namespace Ayla
{
	enum class ShaderType;

	ACLASS()
	class RENDERCORE_API Shader : public Object
	{
		GENERATED_BODY()

	public:
		virtual bool Has(ShaderType type) const = 0;
	};
}