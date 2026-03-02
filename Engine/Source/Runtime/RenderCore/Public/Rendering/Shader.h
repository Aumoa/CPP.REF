// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Shader.gen.h"

namespace Ayla
{
	ACLASS()
	class RENDERCORE_API Shader : public Object
	{
		GENERATED_BODY()

	public:
		virtual String GetEntrypointName() const = 0;
		virtual const byte* GetBytecode() const = 0;
		virtual size_t GetBytecodeSize() const = 0;
	};
}