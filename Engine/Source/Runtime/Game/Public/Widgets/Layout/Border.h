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

	virtual Vector2 GetDesiredSize() const override;

public:
	void SetPadding(const Margin& padding);
	Margin GetPadding() const;
	void SetContent(SWidget* content);
	SWidget* GetContent() const;

protected:
	virtual void OnArrangeChildren(SArrangedChildrens* arrangedChildrens, const Geometry& allottedGeometry) const override;

public:
	BEGIN_SLATE_ATTRIBUTE
		DECLARE_SLATE_ATTRIBUTE(Margin, Padding)
		DECLARE_SLATE_CONTENT(SWidget*)
	END_SLATE_ATTRIBUTE

	DECLARE_SLATE_CONSTRUCTOR();
};