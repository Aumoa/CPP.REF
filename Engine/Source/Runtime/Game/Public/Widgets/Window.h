// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "CompoundWidget.h"

interface IFrameworkView;
class SViewport;

class GAME_API SWindow : public SCompoundWidget
{
	GENERATED_BODY(SWindow)

private:
	IFrameworkView* FrameworkView = nullptr;
	SViewport* GameViewport = nullptr;

public:
	SWindow();

	void InitViewport(IFrameworkView* InFrameworkView);
	SViewport* GetGameViewport();

	virtual Vector2 GetDesiredSize() override;

protected:
	virtual void OnArrangeChildren(SArrangedChildrens* ArrangedChildrens, const Geometry& AllottedGeometry) override;
};