// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RenderPipeline.gen.h"

namespace Ayla
{
	class CommandBuffer;
	class Buffer;

	ACLASS()
	class RENDERCORE_API RenderPipeline : public Object
	{
		GENERATED_BODY()

	public:
		virtual void SetCameraBufferView(CommandBuffer* cmd, Buffer* buffer, size_t offset) = 0;
	};
}