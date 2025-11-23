// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommandBuffer.gen.h"

namespace Ayla
{
	ACLASS()
	class RENDERCORE_API CommandBuffer : public Object
	{
		GENERATED_BODY()

	protected:
		CommandBuffer();

	public:
		virtual ~CommandBuffer() noexcept;

		AFUNCTION()
		virtual void BeginCommands() APURE;
		AFUNCTION()
		virtual void EndCommands() APURE;
	};
}