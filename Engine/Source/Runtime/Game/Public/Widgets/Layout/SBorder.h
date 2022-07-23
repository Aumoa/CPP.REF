// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Widgets/SCompoundWidget.h"

class GAME_API SBorder : public SCompoundWidget
{
	using This = SBorder;
	using Super = SCompoundWidget;

private:
	Margin Padding;
	std::shared_ptr<SWidget> Content = nullptr;

public:
	SBorder();

public:
	void SetPadding(const Margin& InPadding);
	Margin GetPadding();
	void SetContent(std::shared_ptr<SWidget> InContent);
	std::shared_ptr<SWidget> GetContent();

protected:
	virtual Vector2 ComputeDesiredSize() override;
	virtual void OnArrangeChildren(ArrangedChildrens& ArrangedChildrens, const Geometry& AllottedGeometry) override;

public:
	BEGIN_SLATE_ATTRIBUTE
		DECLARE_SLATE_ATTRIBUTE(Margin, Padding)
		DECLARE_SLATE_CONTENT(std::shared_ptr<SWidget>)
	END_SLATE_ATTRIBUTE

	DECLARE_SLATE_CONSTRUCTOR();
};