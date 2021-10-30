// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Window.h"

class SImage;

class REFERENCEVIEWER_API SCoreWindow : public SWindow
{
	GENERATED_BODY(SCoreWindow)

public:
	SCoreWindow();

protected:
	virtual void OnArrangeChildren(ArrangedChildrens& InoutArrangedChildrens, const Geometry& AllottedGeometry) override;

public:
	DECLARE_SLATE_CONSTRUCTOR();

private:
	SImage* Image = nullptr;
};