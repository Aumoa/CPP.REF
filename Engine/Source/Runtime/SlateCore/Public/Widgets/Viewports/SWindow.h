// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "SWindow.gen.h"

class SViewport;
class RHISwapChain;
class RHIDevice;
class RHICommandQueue;
class RHICommandList;
class GameRenderSubsystem;
struct SceneRenderContext;
interface IPlatformWindow;

SCLASS()
class GAME_API SWindow : public SCompoundWidget
{
	GENERATED_BODY()

private:
	std::shared_ptr<RHIDevice> Device;
	std::shared_ptr<RHICommandQueue> CommandQueue;
	WeakPtr<IPlatformWindow> PlatformWindow;

	std::shared_ptr<RHISwapChain> SwapChain;
	Vector2N LastSwapChainSize = Vector2N::Zero();
	std::shared_ptr<SViewport> GameViewport;

	Vector2N CachedDrawingSize;
	TimeSpan CachedDeltaTime;

	std::shared_ptr<RHICommandList> WindowCmdList;

public:
	SWindow();
	virtual ~SWindow() noexcept override;

	virtual void Tick(const Geometry& AllottedGeometry, const TimeSpan& deltaTime) override;
	void DispatchSlateTick(const TimeSpan& deltaTime);
	void PresentWindow(SceneRenderContext& Context);

	virtual size_t NumChildrens() override;
	virtual SWidget* GetChildrenAt(size_t IndexOf) override;

	std::shared_ptr<RHIDevice> GetDevice() const;

protected:
	virtual void OnArrangeChildren(ArrangedChildrens& InoutArrangedChildrens, const Geometry& AllottedGeometry) override;

public:
	BEGIN_SLATE_ATTRIBUTE
		DECLARE_SLATE_ATTRIBUTE(GameRenderSubsystem*, RenderSystem)
		DECLARE_SLATE_ATTRIBUTE(IPlatformWindow*, TargetWindow)
	END_SLATE_ATTRIBUTE;

	DECLARE_SLATE_CONSTRUCTOR();

public:
	DECLARE_MULTICAST_EVENT(SwapChainResizedEvent, Vector2N);
	SwapChainResizedEvent SwapChainResized;

private:
	void TryResizeSwapChain(const Geometry& allottedGeometry);
};