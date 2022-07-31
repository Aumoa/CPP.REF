// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "LayoutEnums.generated.h"

SENUM()
enum class EFlowDirection
{
	LeftToRight,
	RightToLeft,
};

SENUM()
enum class EOrientation
{
	Horizontal,
	Vertical,
};

SENUM()
enum class EWidgetClipping
{
	Inherit,
	ClipToBounds,
};

SENUM()
enum class EHorizontalAlignment
{
	Fill,
	Left,
	Center,
	Right
};

SENUM()
enum class EVerticalAlignment
{
	Fill,
	Top,
	Center,
	Bottom,
};

SENUM()
enum class ESizeRule
{
	Auto,
	Stretch,
};

namespace ESlateVisibility
{
	enum Enum
	{
		Visible,
		Collapsed,
		Hidden,
		HitTestInvisible,
		SelfHitTestInvisible,
		All
	};

	inline constexpr int32 VISPRIVATE_Visible = 0x1 << 0;
	inline constexpr int32 VISPRIVATE_Collapsed = 0x1 << 1;
	inline constexpr int32 VISPRIVATE_Hidden = 0x1 << 2;
	inline constexpr int32 VISPRIVATE_SelfHitTestVisible = 0x1 << 3;
	inline constexpr int32 VISPRIVATE_ChildrenHitTestVisible = 0x1 << 4;

	inline constexpr int32 VIS_Visible = VISPRIVATE_Visible | VISPRIVATE_SelfHitTestVisible | VISPRIVATE_ChildrenHitTestVisible;
	inline constexpr int32 VIS_Collapsed = VISPRIVATE_Collapsed;
	inline constexpr int32 VIS_Hidden = VISPRIVATE_Hidden;
	inline constexpr int32 VIS_HitTestInvisible = VISPRIVATE_Visible;
	inline constexpr int32 VIS_SelfHitTestInvisible = VISPRIVATE_Visible | VISPRIVATE_ChildrenHitTestVisible;

	inline constexpr int32 VIS_All = VISPRIVATE_Visible | VISPRIVATE_Hidden | VISPRIVATE_Collapsed | VISPRIVATE_SelfHitTestVisible | VISPRIVATE_ChildrenHitTestVisible;

	inline int32 GetValue(ESlateVisibility::Enum visibility)
	{
		switch (visibility)
		{
		case ESlateVisibility::Visible: return VIS_Visible;
		case ESlateVisibility::Collapsed: return VIS_Collapsed;
		case ESlateVisibility::Hidden: return VIS_Hidden;
		case ESlateVisibility::HitTestInvisible: return VIS_HitTestInvisible;
		case ESlateVisibility::SelfHitTestInvisible: return VIS_SelfHitTestInvisible;
		case ESlateVisibility::All: return VIS_All;
		default:
			throw Exception(String::Format(TEXT("Invalid argument: Visibility({})"), (int32)visibility));
		};
	}

	inline String ToString(ESlateVisibility::Enum visibility)
	{
		switch (visibility)
		{
		case ESlateVisibility::Visible: return TEXT("Visible");
		case ESlateVisibility::Collapsed: return TEXT("Collapsed");
		case ESlateVisibility::Hidden: return TEXT("Hidden");
		case ESlateVisibility::HitTestInvisible: return TEXT("HitTestInvisible");
		case ESlateVisibility::SelfHitTestInvisible: return TEXT("SelfHitTestInvisible");
		case ESlateVisibility::All: return TEXT("All");
		default: return String::Format(TEXT("(ESlateVisibility){}"), (int32)visibility);
		};
	}

	inline bool DoesVisibilityPassFilter(ESlateVisibility::Enum visibility, ESlateVisibility::Enum filter)
	{
		return 0 != (GetValue(visibility) & GetValue(filter));
	}

	inline bool AreChildrenHitTestVisible(ESlateVisibility::Enum visibility)
	{
		return 0 != (GetValue(visibility) & VISPRIVATE_ChildrenHitTestVisible);
	}

	inline bool IsHitTestVisible(ESlateVisibility::Enum visibility)
	{
		return 0 != (GetValue(visibility) & VISPRIVATE_SelfHitTestVisible);
	}

	inline bool IsVisible(ESlateVisibility::Enum visibility)
	{
		return 0 != (GetValue(visibility) & VIS_Visible);
	}
}