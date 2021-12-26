// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/CompoundWidget.h"

interface IApplicationInterface;
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

	virtual Vector2 GetDesiredSize() override;

protected:
	virtual void OnArrangeChildren(ArrangedChildrens& ArrangedChildrens, const Geometry& AllottedGeometry) override;
};