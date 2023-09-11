// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Widgets/SCompoundWidget.h"

class NGenericWindow;
class NRHIViewport;
class SViewport;

class SLATECORE_API SWindow : public SCompoundWidget
{
private:
	std::shared_ptr<NGenericWindow> NativeWindow;
	std::shared_ptr<NRHIViewport> Viewport;
	std::vector<std::shared_ptr<SViewport>> SlateViewports;

public:
	SWindow();
	virtual ~SWindow() noexcept override;

	void AttachWindow(std::shared_ptr<NGenericWindow> InNativeWindow);
	void Present();

	void AddViewport(std::shared_ptr<SViewport> InViewport);

public:
	BEGIN_SLATE_ATTRIBUTE(SWindow)
	END_SLATE_ATTRIBUTE();

	DECLARE_SLATE_CONSTRUCTOR();

protected:
	virtual Vector2 ComputeDesiredSize() const override;

	virtual int32 OnPaint(const NPaintArgs& Args, const NGeometry& AllottedGeometry, const Rect& CullingRect, NSlateWindowElementList& OutDrawElements, int32 InLayer, bool bParentEnabled) const override;

	virtual void OnVisibilityChanged(ESlateVisibility::Enum Prev, ESlateVisibility::Enum New);

private:
	void UpdateWindowVisibility();
};