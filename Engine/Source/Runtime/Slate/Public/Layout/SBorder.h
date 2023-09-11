// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Layout/Margin.h"

class SLATE_API SBorder : public SCompoundWidget
{
	GENERATED_SLATE_BODY(SBorder)

private:
	NMargin Padding;
	std::shared_ptr<SWidget> Content;

public:
	SBorder();
	virtual ~SBorder() noexcept override;

	void SetPadding(const NMargin& InPadding);
	NMargin GetPadding() const { return Padding; }

	void SetContent(std::shared_ptr<SWidget> InContent);
	std::shared_ptr<SWidget> GetContent() const { return Content; }

protected:
	virtual Vector2 ComputeDesiredSize() const override;
	virtual void OnArrangeChildren(NArrangedChildrens& ArrangedChildrens, const NGeometry& AllottedGeometry) const override;

public:
	BEGIN_SLATE_ATTRIBUTE(SBorder)
		DECLARE_SLATE_ATTRIBUTE(NMargin, Padding)
		DECLARE_SLATE_CONTENT(std::shared_ptr<SWidget>)
	END_SLATE_ATTRIBUTE();

	DECLARE_SLATE_CONSTRUCTOR();
};