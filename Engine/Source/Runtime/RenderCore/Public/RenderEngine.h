// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

namespace libty::inline RenderCore
{
	class SRenderContext;

	class RENDERCORE_API SRenderEngine : virtual public SObject
	{
		GENERATED_BODY(SRenderEngine);

	private:
		SPROPERTY(RHIDevice)
		IRHIDevice* RHIDevice = nullptr;
		SPROPERTY(RHIQueue)
		IRHICommandQueue* RHIQueue = nullptr;

	public:
		SRenderEngine();

		virtual SRenderContext* CreateRenderContext();
	};
}