// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/CompoundWidget.h"

struct IApplicationInterface;
class SViewport;
class SDebugCanvas;

class GAME_API SWindow : public SCompoundWidget
{
	GENERATED_BODY(SWindow)

private:
	SPROPERTY(GameViewport)
	SViewport* GameViewport = nullptr;

public:
	SWindow();

	virtual void InitViewport();
	SViewport* GetGameViewport();

protected:
	virtual Vector2 ComputeDesiredSize() override;
	virtual void OnArrangeChildren(ArrangedChildrens& ArrangedChildrens, const Geometry& AllottedGeometry) override;

	virtual size_t NumChildrens();
	virtual SWidget* GetChildrenAt(size_t IndexOf) override;
};