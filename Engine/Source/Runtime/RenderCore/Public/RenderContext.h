// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

namespace libty::inline RenderCore
{
	class RENDERCORE_API SRenderContext : virtual public SObject
	{
		GENERATED_BODY(SRenderContext);

	public:
		SRenderContext();

		SPROPERTY(OwningDevice)
		IRHIDevice* OwningDevice = nullptr;
		SPROPERTY(RenderQueue)
		IRHICommandQueue* RenderQueue = nullptr;
		SPROPERTY(CommandAllocator)
		IRHICommandAllocator* CommandAllocator = nullptr;
		SPROPERTY(CommandList)
		IRHIGraphicsCommandList* CommandList = nullptr;
	};
}