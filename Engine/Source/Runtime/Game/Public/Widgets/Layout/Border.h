// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/CompoundWidget.h"
#include "Layout/Margin.h"

class GAME_API SBorder : public SCompoundWidget
{
	GENERATED_BODY(SBorder)

private:
	Margin _padding;
	SWidget* _content = nullptr;

public:
	SBorder(const std::wstring& name);

	virtual Vector2 GetDesiredSize() override;

public:
	void SetPadding(const Margin& padding);
	Margin GetPadding();
	void SetContent(SWidget* content);
	SWidget* GetContent();

protected:
	virtual void OnArrangeChildren(SArrangedChildrens* arrangedChildrens, const Geometry& allottedGeometry) override;

public:
	BEGIN_SLATE_ATTRIBUTE
		DECLARE_SLATE_ATTRIBUTE(Margin, Padding)
		DECLARE_SLATE_CONTENT(SWidget*)
	END_SLATE_ATTRIBUTE

	DECLARE_SLATE_CONSTRUCTOR();
};