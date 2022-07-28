// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Widgets/SCompoundWidget.h"

class SViewport;
class RHISwapChain;
class RHIDevice;
class RHICommandQueue;
class GameRenderSubsystem;
interface IPlatformWindow;

class GAME_API SWindow : public SCompoundWidget
{
	using This = SWindow;
	using Super = SCompoundWidget;

private:
	std::shared_ptr<RHIDevice> _device;
	std::shared_ptr<RHICommandQueue> _commandQueue;
	WeakPtr<IPlatformWindow> _platformWindow;

	std::shared_ptr<RHISwapChain> _swapChain;
	std::vector<std::shared_ptr<SViewport>> _viewports;

public:
	SWindow();

	virtual void Tick(const Geometry& AllottedGeometry, const TimeSpan& deltaTime) override;
	void DispatchSlateTick(const TimeSpan& deltaTime);
	void PresentWindow();

	virtual size_t NumChildrens() override;
	virtual SWidget* GetChildrenAt(size_t IndexOf) override;

protected:
	virtual void OnArrangeChildren(ArrangedChildrens& InoutArrangedChildrens, const Geometry& AllottedGeometry) override;

public:
	BEGIN_SLATE_ATTRIBUTE
		DECLARE_SLATE_ATTRIBUTE(GameRenderSubsystem*, RenderSystem)
		DECLARE_SLATE_ATTRIBUTE(IPlatformWindow*, TargetWindow)
	END_SLATE_ATTRIBUTE;

	DECLARE_SLATE_CONSTRUCTOR();

private:
	void TryResizeSwapChain(const Geometry& allottedGeometry);
};