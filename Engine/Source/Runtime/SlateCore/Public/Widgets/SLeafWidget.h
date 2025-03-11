// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SWidget.h"

class SLATECORE_API SLeafWidget : public SWidget
{
	GENERATED_SLATE_BODY(SLeafWidget)

public:
	SLeafWidget();

protected:
	virtual void OnArrangeChildren(NArrangedChildrens& InoutArrangedChildrens, const NGeometry& AllottedGeometry) const override;

public:
	BEGIN_SLATE_ATTRIBUTE()
	END_SLATE_ATTRIBUTE();

	DECLARE_SLATE_CONSTRUCTOR() {}
};