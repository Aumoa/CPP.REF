// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/CompoundWidget.h"

class SLATECORE_API SBorder : public SCompoundWidget
{
	GENERATED_BODY(SBorder)

private:
	Margin Padding;
	SWidget* Content = nullptr;

public:
	SBorder();

	virtual Vector2 GetDesiredSize() override;

public:
	void SetPadding(const Margin& InPadding);
	Margin GetPadding();
	void SetContent(SWidget* InContent);
	SWidget* GetContent();

protected:
	virtual void OnArrangeChildren(ArrangedChildrens& ArrangedChildrens, const Geometry& AllottedGeometry) override;

public:
	BEGIN_SLATE_ATTRIBUTE
		DECLARE_SLATE_ATTRIBUTE(Margin, Padding)
		DECLARE_SLATE_CONTENT(SWidget*)
	END_SLATE_ATTRIBUTE

	DECLARE_SLATE_CONSTRUCTOR();
};