// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommandBuffer.gen.h"

namespace Ayla
{
	class RenderPipeline;
	class RenderTexture;

	ACLASS()
	class RENDERCORE_API CommandBuffer : public Object
	{
		GENERATED_BODY()

	protected:
		CommandBuffer();

	public:
		virtual ~CommandBuffer() noexcept;

		virtual void Dispose() noexcept = 0;

		AFUNCTION()
		virtual void BeginCommands() APURE;
		AFUNCTION()
		virtual void EndCommands() APURE;
		virtual void BeginRenderPass(RenderTexture* renderTexture) = 0;
		virtual void EndRenderPass(RenderTexture* renderTexture) = 0;
		virtual void SetRenderPipeline(RenderPipeline* renderPipeline) = 0;
		virtual void Draw() = 0;
		virtual void DispatchRays(RenderTexture* renderTexture) = 0;
		virtual void WaitForCompletion(const TimeSpan& timeout) = 0;
	};
}
