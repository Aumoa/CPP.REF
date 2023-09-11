// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Widgets/SWidget.h"
#include "Layout/ArrangedChildrens.h"

class SLATECORE_API SCompoundWidget : public SWidget
{
	GENERATED_SLATE_BODY(SCompoundWidget)

public:
	SCompoundWidget();

protected:
	virtual void OnArrangeChildren(NArrangedChildrens& InoutArrangedChildrens, const NGeometry& AllottedGeometry) const = 0;

public:
	BEGIN_SLATE_ATTRIBUTE(SCompoundWidget)
	END_SLATE_ATTRIBUTE();

	DECLARE_SLATE_CONSTRUCTOR() {}
};