// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class NGenericWindow;
class NRHISwapChain;
class NSlateRenderer;

class SLATECORE_API SWindow : public SCompoundWidget
{
	GENERATED_SLATE_BODY(SWindow)

private:
	std::shared_ptr<NGenericWindow> NativeWindow;
	std::shared_ptr<NRHISwapChain> SwapChain;

public:
	SWindow();
	virtual ~SWindow() noexcept override;

	void AttachWindow(std::shared_ptr<NGenericWindow> InNativeWindow);
	void ExecuteTick(const TimeSpan& InDeltaTime, NSlateWindowElementList& OutDrawElements, const std::vector<NGenericPlatformInputEvent>& InputEvents);
	void Render(const NSlateWindowElementList& DrawElements, NSlateRenderer& Renderer);
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