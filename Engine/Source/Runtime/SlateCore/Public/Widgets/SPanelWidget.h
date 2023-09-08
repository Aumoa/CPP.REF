// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SWidget.h"
#include "Layout/ArrangedChildrens.h"

class SLATECORE_API SPanelWidget : public SWidget
{
public:
	SPanelWidget();

public:
	virtual size_t NumChildrens() const = 0;
	virtual SWidget* GetChildrenAt(size_t InIndex) const = 0;

public:
	BEGIN_SLATE_ATTRIBUTE(SPanelWidget)
	END_SLATE_ATTRIBUTE()

	DECLARE_SLATE_CONSTRUCTOR() {}

protected:
	virtual void OnArrangeChildren(NArrangedChildrens& InoutArrangedChildrens, const NGeometry& AllottedGeometry) const = 0;
};