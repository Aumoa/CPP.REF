// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

SENUM(EFlowDirection, int32,
	LeftToRight,,
	RightToLeft,
);

SENUM(EOrientation, int32,
	Horizontal,,
	Vertical,
);

SENUM(EWidgetClipping, int32,
	Inherit,,
	ClipToBounds,
);

SENUM(EHorizontalAlignment, int32,
	Fill,,
	Left,,
	Center,,
	Right,
);

SENUM(EVerticalAlignment, int32,
	Fill,,
	Top,,
	Center,,
	Bottom,
);

SENUM(ESizeRule, int32,
	Auto,,
	Stretch,
);

SENUM_BEGIN(ESlateVisibility, int32,
	Visible, ,
	Collapsed, ,
	Hidden, ,
	HitTestInvisible, ,
	SelfHitTestInvisible, ,
	All,
	)

private:
	static constexpr int32 VISPRIVATE_Visible = 0x1 << 0;
	static constexpr int32 VISPRIVATE_Collapsed = 0x1 << 1;
	static constexpr int32 VISPRIVATE_Hidden = 0x1 << 2;
	static constexpr int32 VISPRIVATE_SelfHitTestVisible = 0x1 << 3;
	static constexpr int32 VISPRIVATE_ChildrenHitTestVisible = 0x1 << 4;

	static constexpr int32 VIS_Visible = VISPRIVATE_Visible | VISPRIVATE_SelfHitTestVisible | VISPRIVATE_ChildrenHitTestVisible;
	static constexpr int32 VIS_Collapsed = VISPRIVATE_Collapsed;
	static constexpr int32 VIS_Hidden = VISPRIVATE_Hidden;
	static constexpr int32 VIS_HitTestInvisible = VISPRIVATE_Visible;
	static constexpr int32 VIS_SelfHitTestInvisible = VISPRIVATE_Visible | VISPRIVATE_ChildrenHitTestVisible;

	static constexpr int32 VIS_All = VISPRIVATE_Visible | VISPRIVATE_Hidden | VISPRIVATE_Collapsed | VISPRIVATE_SelfHitTestVisible | VISPRIVATE_ChildrenHitTestVisible;

public:
	static bool DoesVisibilityPassFilter(ESlateVisibility Visibility, ESlateVisibility Filter)
	{
		return 0 != (GetValue(Visibility) & GetValue(Filter));
	}

	bool DoesVisibilityPassFilter(ESlateVisibility Filter) const
	{
		return DoesVisibilityPassFilter(*this, Filter);
	}

	static bool AreChildrenHitTestVisible(ESlateVisibility Visibility)
	{
		return 0 != (GetValue(Visibility) & VISPRIVATE_ChildrenHitTestVisible);
	}

	bool AreChildrenHitTestVisible() const
	{
		return AreChildrenHitTestVisible(*this);
	}

	static bool IsHitTestVisible(ESlateVisibility Visibility)
	{
		return 0 != (GetValue(Visibility) & VISPRIVATE_SelfHitTestVisible);
	}

	bool IsHitTestVisible() const
	{
		return IsHitTestVisible(*this);
	}

	static bool IsVisible(ESlateVisibility Visibility)
	{
		return 0 != (GetValue(Visibility) & VIS_Visible);
	}

	bool IsVisible() const
	{
		return IsVisible(*this);
	}

private:
	static int32 GetValue(ESlateVisibility Visibility)
	{
		switch (Visibility)
		{
		case ESlateVisibility::Visible: return VIS_Visible;
		case ESlateVisibility::Collapsed: return VIS_Collapsed;
		case ESlateVisibility::Hidden: return VIS_Hidden;
		case ESlateVisibility::HitTestInvisible: return VIS_HitTestInvisible;
		case ESlateVisibility::SelfHitTestInvisible: return VIS_SelfHitTestInvisible;
		case ESlateVisibility::All: return VIS_All;
		default:
			throw fatal_exception(String::Format("Invalid argument: Visibility({})", (int32)Visibility));
		};
	}

	int32 GetValue() const
	{
		return GetValue(*this);
	}
SENUM_END()
