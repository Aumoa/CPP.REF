// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Shader.gen.h"

namespace Ayla
{
	enum class ShaderType;
	struct ShaderBytecode;
	class VertexFactory;

	ACLASS()
	class RENDERCORE_API Shader : public Object
	{
		GENERATED_BODY()

	public:
		virtual VertexFactory* GetVertexFactory() const = 0;
		virtual bool Has(ShaderType type) const = 0;
		virtual const ShaderBytecode& GetBytecode(ShaderType type) const = 0;
	};
}