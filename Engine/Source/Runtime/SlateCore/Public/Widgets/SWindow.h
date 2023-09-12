// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Widgets/SCompoundWidget.h"

class NGenericWindow;
class NRHIViewport;
class SViewport;

class SLATECORE_API SWindow : public SCompoundWidget
{
	GENERATED_SLATE_BODY(SWindow)

private:
	std::shared_ptr<NGenericWindow> NativeWindow;
	std::shared_ptr<NRHIViewport> Viewport;
	std::shared_ptr<SViewport> SlateViewport;

public:
	SWindow();
	virtual ~SWindow() noexcept override;

	virtual void Tick(const NGeometry& AllottedGeomtry, const TimeSpan& InDeltaTime);

	void AttachWindow(std::shared_ptr<NGenericWindow> InNativeWindow);
	void ExecuteTick(const TimeSpan& InDeltaTime);
	void Present();

public:
	BEGIN_SLATE_ATTRIBUTE()
	END_SLATE_ATTRIBUTE();

	DECLARE_SLATE_CONSTRUCTOR();

protected:
	virtual Vector2 ComputeDesiredSize() const override;

	virtual void OnVisibilityChanged(ESlateVisibility::Enum Prev, ESlateVisibility::Enum New);

private:
	void UpdateWindowVisibility();
};