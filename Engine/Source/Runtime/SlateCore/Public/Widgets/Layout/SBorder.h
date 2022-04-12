// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Widgets/SCompoundWidget.h"

namespace libty::inline SlateCore
{
	class SLATECORE_API SBorder : extends(SCompoundWidget)
	{
		GENERATED_BODY(SBorder);

	private:
		Margin Padding;
		SPROPERTY(Content)
		SWidget* Content = nullptr;

	public:
		SBorder();

	public:
		void SetPadding(const Margin& InPadding);
		Margin GetPadding();
		void SetContent(SWidget* InContent);
		SWidget* GetContent();

	protected:
		virtual Vector2 ComputeDesiredSize() override;
		virtual void OnArrangeChildren(ArrangedChildrens& ArrangedChildrens, const Geometry& AllottedGeometry) override;

	public:
		BEGIN_SLATE_ATTRIBUTE
			DECLARE_SLATE_ATTRIBUTE(Margin, Padding)
			DECLARE_SLATE_CONTENT(SWidget*)
		END_SLATE_ATTRIBUTE

		DECLARE_SLATE_CONSTRUCTOR();
	};
}