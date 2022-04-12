// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

namespace libty::inline SlateCore
{
	class SWidget;
	class SceneRenderTarget;

	class SLATECORE_API PaintArgs
	{
	public:
		IRHIDevice* const Device;
		SWidget*	const Parent;
		float		const DeltaTime;
		float		const RenderOpacity;

	private:
		PaintArgs(IRHIDevice* Device, SWidget* InParentWidget, float InDeltaTime, float RenderOpacity = 1.0f);

	public:
		static PaintArgs InitPaintArgs(IRHIDevice* Device, float InDeltaTime);
		PaintArgs WithNewParent(SWidget* InParentWidget) const;
	};
}